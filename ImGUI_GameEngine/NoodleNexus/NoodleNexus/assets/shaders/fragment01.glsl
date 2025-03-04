#version 330
// (Pixel) Fragment fragment
in vec3 fColour;			// Actual 3D model colour (from vertex buffer)
in vec4 fvertexWorldLocation;
in vec4 fvertexNormal;
in vec2 fUV;				// Texture (UV) coordinates

uniform vec4 objectColour;			// Override colour 
uniform bool bUseObjectColour;
uniform vec4 eyeLocation;			// Where the camera is
uniform bool bDoNotLight;			// if true, skips lighting
// 0.0 to 1.0 (invisible to solid)
// Controls the alpha channel
uniform float wholeObjectTransparencyAlpha;

// Written to the framebuffer (backbuffer)
out vec4 finalPixelColour;	// RGB_A

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;


struct sLight
{
	vec4 position;			
	vec4 diffuse;	
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
	                // yzw are TBD
};

const int NUMBEROFLIGHTS = 20;
uniform sLight theLights[NUMBEROFLIGHTS]; 
// uniform vec4 thelights[0].position;
// uniform vec4 thelights[1].position;

// Inspired by Mike Bailey's Graphic Shader, chapter 6
// (you should read it. Yes, you)
vec4 calculateLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular );

// Allows us to lookup the RGB colour from a 2D texture
// Give it the UV and it returns the colour at that UV location
uniform sampler2D texture00;
uniform sampler2D texture01;
uniform sampler2D texture02;
uniform sampler2D texture03;
uniform vec4 texRatio_0_to_3;	// x index 0, y index 1, etc/
//uniform float texRatio[4];
uniform bool bUseTextureAsColour;	// If true, then sample the texture

// Skybox (or reflection, refraction, etc.)
uniform samplerCube skyBoxTextureSampler;
uniform bool bIsSkyBoxObject;
//
uniform bool b_Is_FBO_Texture;
//

// For discard stencil example
uniform sampler2D stencilTexture;
uniform bool bUseStencilTexture;

//uniform sampler2D textures[4];
//uniform sampler2DArray textures[3]



void main()
{
	// discard transparency
	// uniform sampler2D stencilTexture;
	// uniform bool bUseStencilTexture;
	if ( bUseStencilTexture )
	{
		float stencilColour = texture( stencilTexture, fUV.st ).r;
		//
		if ( stencilColour < 0.5f )	// Is it "black enough"
		{
			discard;	// don't draw this pixel
		}
	}


	// For the skybox object
	if ( bIsSkyBoxObject )
	{
		//finalPixelColour.rgb = fvertexNormal.xyz;
		//uniform samplerCube skyBoxTextureSampler;
		// Note: We are passing the NORMALS (a ray to hit the inside
		// 	of the cube) and NOT the texture coordinates
		finalPixelColour.rgb = texture( skyBoxTextureSampler, fvertexNormal.xyz ).rgb;
		finalPixelColour.a = 1.0f;
		return;
	}
	
	



	vec3 vertexColour = fColour;
	if ( bUseObjectColour )
	{
		vertexColour = objectColour.rgb;
	}
	
	if ( bUseTextureAsColour )
	{
//			uniform sampler2D texture00;
//		uniform sampler2D texture01;
//		uniform sampler2D texture02;
//		uniform sampler2D texture03;
//		uniform vec4 texRatio_0_to_3;	// x index 0, y index 1, etc/
	

//	// chromatic aberration
//		vec2 UV_Offset_R = vec2(fUV.s - 0.01f, fUV.t + 0.01f);
//		vec2 UV_Offset_G = vec2(fUV.s - 0.01f, fUV.t - 0.01f);
//		vec2 UV_Offset_B = vec2(fUV.s + 0.01f, fUV.t + 0.01f);
//		
//		vec3 texColour00 = vec3(0.0f);
//		texColour00.r = texture( texture00, UV_Offset_R ).r;
//		texColour00.g = texture( texture00, UV_Offset_G ).g;
//		texColour00.b = texture( texture00, UV_Offset_B ).b;

		vec3 texColour00 = texture( texture00, fUV.st ).rgb;
//		vec2 screenRollUV = fUV.st;
//		fUV.t += rollAmount;
//		vec3 texColour00 = texture( texture00, screenRollUV ).rgb;
		
		vec3 texColour01 = texture( texture01, fUV.st ).rgb;	
		vec3 texColour02 = texture( texture02, fUV.st ).rgb;	
		vec3 texColour03 = texture( texture03, fUV.st ).rgb;	
		
		
		// All these ratios should add up to 1.0
		vertexColour.rgb =   (texColour00.rgb * texRatio_0_to_3.x)
		                   + (texColour01.rgb * texRatio_0_to_3.y)
		                   + (texColour02.rgb * texRatio_0_to_3.z)
		                   + (texColour03.rgb * texRatio_0_to_3.w);
				
				
//		vertexColour.rgb = texColour00.rgb * (1.0f - texDirtyGlass.r)	
//                           + texDirtyGlass.rgb;	
		// Use #2 texture to modulate the 1st texture		
//		vertexColour.rgb =   (texColour03.rgb * texColour02.r) 
//		                   + (texColour00.rgb * (1.0f - texColour02.r));
						   
	} 
	

	if ( b_Is_FBO_Texture )
	{
//		vec3 texColour00 = texture( texture00, fUV.st ).rgb;
//		vertexColour.rgb = texColour00.rgb;	
		//     -1   0   +1
		//  -1  .   +    .
		//   0  +   O    +
		//  +1  .   +    .
		//
		// Kernel size is 3 or 3x3 
		// size of 5 would 5 samples x 5 samples = 25
		// 
		// +++++
		// +++++
		// ++O++
		// +++++
		// +++++
		
		// ....+.....
		// ....+.....
		// ....+.....
		// ++++O+++++
		// ....+.....
		// ....+.....
		// ....+.....
		
		// Note: While you CAN pass multi-dimensional arrays
		//       through uniform variables, you CAN'T as constant arrays.
		//
		// int[5][5] gaussian_blur_5x5;		// Illegal
		//
		// So I'm doing this strange thing:
		struct sRow
		{
			int column[5];
		};
		
		sRow gaussian_5x5[5];
		// I took the 5x5 kernel from here:
//		// https://en.wikipedia.org/wiki/Kernel_(image_processing)
		gaussian_5x5[0].column = int[](1,  4,  6,  4, 1);	// Note strange array init
		gaussian_5x5[1].column = int[](4, 16, 24, 16, 1);	// Note strange array init
		gaussian_5x5[2].column = int[](6, 24, 36, 24, 6);	// Note strange array init
		gaussian_5x5[3].column = int[](4, 16, 24, 16, 1);	// Note strange array init
		gaussian_5x5[4].column = int[](1,  4,  6,  4, 1);	// Note strange array init
		// There are a total of 256 samples in this gaussian 
		
		vertexColour = vec3(0.0f, 0.0f, 0.0f);	// black
		
		
		
		
		
		const float OFFSET_UV = 1.0f/1920.0f;	// Screen is 1920x1080
		
		int samplesTaken = 0;
		
		// 3x3 starts at -1 less than 2
		// 5x5 starts at -2 less than 3
		// 9x9 starts at -4 less than 5  --> 81 samples
		for ( int xIndex = -2; xIndex < 3; xIndex++ )
		{
			for ( int yIndex = -2; yIndex < 3; yIndex++ )
			{
				vec2 UV_Offset = vec2(0.0f);
				UV_Offset.s = fUV.s + (xIndex * OFFSET_UV);
				UV_Offset.t = fUV.t + (yIndex * OFFSET_UV);
				//vertexColour.rgb += texture( texture00, UV_Offset).rgb;

				// Multiple the sample by the value in the [x][y] gaussian matrix above
				vec3 sampleColour = texture( texture00, UV_Offset).rgb;
				int gassianWeight = gaussian_5x5[xIndex + 2].column[yIndex + 2];
				vertexColour.rgb += ( sampleColour * gassianWeight );

//				samplesTaken++;
				samplesTaken += gassianWeight;
				
			}//for ( int yIndex...
		}// for ( int xIndex...	

		vertexColour.rgb /= float(samplesTaken);

	}//if ( b_Is_FBO_Texture )

	
	
//	finalPixelColour = vec4(finalColour, 1.0);

//	vec3 fvertexNormal = vec3(0.0f, 1.0f, 0.0f);
	vec4 vertexSpecular = vec4(1.0f, 1.0f, 1.0f, 1.0f);	


	// Use lighting?
	if ( bDoNotLight )
	{
		finalPixelColour.rgb = vertexColour.rgb;
		finalPixelColour.a = wholeObjectTransparencyAlpha;
		return;
	}
	

	vec4 pixelColour = calculateLightContrib( vertexColour.rgb, 
	                                          fvertexNormal.xyz, 
	                                          fvertexWorldLocation.xyz, 
											  vertexSpecular );

											
	finalPixelColour = pixelColour;
	// Set the alpha channel
	finalPixelColour.a = wholeObjectTransparencyAlpha;	
	
//	// Reflection:
//	vec3 eyeToVertexRay = normalize(eyeLocation.xyz - fvertexWorldLocation.xyz);
//	
//	vec3 reflectRay = reflect(eyeToVertexRay, fvertexNormal.xyz);	
//	vec3 refractRay = refract(eyeToVertexRay, fvertexNormal.xyz, 1.2f);
//	
//	vec3 reflectColour = texture( skyBoxTextureSampler, reflectRay.xyz ).rgb;
//	vec3 refractColour = texture( skyBoxTextureSampler, refractRay.xyz ).rgb;
//	
//	finalPixelColour.rgb += reflectColour.rgb * 0.3f
//	                       + refractColour.rgb * 0.3f;
						   
	return;

	
	
	
	// Use the 4th value (alpha) as the transparency
//	finalPixelColour.a = 1.0f - texture( texture03, fUV.st ).r;

//	finalPixelColour.a = 1.0f;		
//	finalPixelColour.a = 0.9f;		

	// Make the actual colour almost black
	// Apply the UVs as a colour
//	finalPixelColour.rgb *= 0.001f;	// Almost black
//	finalPixelColour.rg += fUV.xy;	// Add the UVs as colours

	// uniform sampler2D texture01;
	//vec3 texColour = texture( texture00, fUV.st ).rgb;
	//finalPixelColour.rgb += texColour;

}


// Inspired by Mike Bailey's Graphic Shader, chapter 6
// (you should read it. Yes, you)
vec4 calculateLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular )
{
	vec3 norm = normalize(vertexNormal);
	
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);
		
		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
		{
			// This is supposed to simulate sunlight. 
			// SO: 
			// -- There's ONLY direction, no position
			// -- Almost always, there's only 1 of these in a scene
			// Cheapest light to calculate. 

			vec3 lightContrib = theLights[index].diffuse.rgb;
			
			// Get the dot product of the light and normalize
			float dotProduct = dot( -theLights[index].direction.xyz,  
									   normalize(norm.xyz) );	// -1 to 1

			dotProduct = max( 0.0f, dotProduct );		// 0 to 1
		
			lightContrib *= dotProduct;		
			
			finalObjectColour.rgb += (vertexMaterialColour.rgb * theLights[index].diffuse.rgb * lightContrib); 
									 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
			// NOTE: There isn't any attenuation, like with sunlight.
			// (This is part of the reason directional lights are fast to calculate)


			return finalObjectColour;		
		}
		
		// Assume it's a point light 
		// intLightType = 0
		
		// Contribution for this light
		vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;
		float distanceToLight = length(vLightToVertex);	
		vec3 lightVector = normalize(vLightToVertex);
		float dotProduct = dot(lightVector, vertexNormal.xyz);	 
		
		// Cut off the light after the distance cut off 
		if ( distanceToLight > theLights[index].atten.w )
		{
			finalObjectColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
			return finalObjectColour;
		}
		
		dotProduct = max( 0.0f, dotProduct );	
		
		vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;
			

		// Specular 
		vec3 lightSpecularContrib = vec3(0.0f);
			
		vec3 reflectVector = reflect( -lightVector, normalize(norm.xyz) );

		// Get eye or view vector
		// The location of the vertex in the world to your eye
		vec3 eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);

		// To simplify, we are NOT using the light specular value, just the object’s.
		float objectSpecularPower = vertexSpecular.w; 
		
//		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
//			                   * vertexSpecular.rgb;	//* theLights[lightIndex].Specular.rgb
		lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
			                   * theLights[index].specular.rgb;
							   
		// Attenuation
		float attenuation = 1.0f / 
				( theLights[index].atten.x + 										
				  theLights[index].atten.y * distanceToLight +						
				  theLights[index].atten.z * distanceToLight*distanceToLight );  	
				  
		// total light contribution is Diffuse + Specular
		lightDiffuseContrib *= attenuation;
		lightSpecularContrib *= attenuation;
		
		
		// But is it a spot light
		if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
		{	
		

			// Yes, it's a spotlight
			// Calcualate light vector (light to vertex, in world)
			vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;

			vertexToLight = normalize(vertexToLight);

			float currentLightRayAngle
					= dot( vertexToLight.xyz, theLights[index].direction.xyz );
					
			currentLightRayAngle = max(0.0f, currentLightRayAngle);

			//vec4 param1;	
			// x = lightType, y = inner angle, z = outer angle, w = TBD

			// Is this inside the cone? 
			float outerConeAngleCos = cos(radians(theLights[index].param1.z));
			float innerConeAngleCos = cos(radians(theLights[index].param1.y));
							
			// Is it completely outside of the spot?
			if ( currentLightRayAngle < outerConeAngleCos )
			{
				// Nope. so it's in the dark
				lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
				lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
			}
			else if ( currentLightRayAngle < innerConeAngleCos )
			{
				// Angle is between the inner and outer cone
				// (this is called the penumbra of the spot light, by the way)
				// 
				// This blends the brightness from full brightness, near the inner cone
				//	to black, near the outter cone
				float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
									  (innerConeAngleCos - outerConeAngleCos);
									  
				lightDiffuseContrib *= penumbraRatio;
				lightSpecularContrib *= penumbraRatio;
			}
						
		}// if ( intLightType == 1 )
		
		
					
		finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
								  + (vertexSpecular.rgb  * lightSpecularContrib.rgb );

	}//for(intindex=0...
	
	finalObjectColour.a = 1.0f;
	
	return finalObjectColour;
}
