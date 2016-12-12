#version 330

          vec4 calculateSpotLight(in vec3 lightVec, in vec3 normalVec, in vec3 eyeVec){
            
            // define (would come from c++ code)
            vec3 spotDirection = vec3(0.0,-1.0,0.0);
            float cutOffAngle = cos(radians(5.0));
            // end define
            
            vec4 color = vec4(1.0,0.0,0.0,1.0);
            float SpotFac = dot(-lightVec,spotDirection);
              
            if( SpotFac > cutOffAngle ){
             float att = clamp(dot(normalVec,-lightVec),0.0,1.0);
             color = att* color * (1.0 - (1.0 - SpotFac) * 1.0/(1.0 - cutOffAngle));
            }else{
              color = vec4(0.0,0.0,0.0,1.0); 
            }
            
            
            return color;
            
          }

          
          //Output
          out vec4 color;

          //Input from vertex shader
          in VS_OUT
          {
          	vec3 N;
          	vec3 L;
          	vec3 V;
            vec3 L2;

            vec3 Ka;
            vec2 texture;
            vec3 Ks;

            vec3 diffuse;
            vec3 scalar;
            vec3 spot;
            vec3 spec;
            vec3 light_color;

            // spotlight
            vec3 spotlightPos;
            vec3 spotLightDir;

          } fs_in;


          //material properties
          uniform float specular_power = 128.0;
          uniform sampler2D gSampler;

          void main(void)
          { 
          	//normalize the incoming N, L, and V vectors
          	vec3 N = normalize(fs_in.N);
          	vec3 L = normalize(fs_in.L);
          	vec3 V = normalize(fs_in.V);

          	//calculate the halfway
          	vec3 R = normalize( reflect(-L, N) );    

            vec4 texel = texture2D(gSampler, fs_in.texture.xy); 

            vec3 diffuse = texel.rgb * fs_in.light_color * max(dot(N,L), 0.0);

          	//compute the diffuse and specular components for each fragment;
          	vec3 specular = pow(max(dot(R,V), 0.0),  specular_power) * fs_in.spec;
            color = vec4(fs_in.scalar + diffuse, 1.0) + vec4 (specular,1);

            //Second Light//////////////////////////

            vec3 L2 = normalize(fs_in.L2);

            //calculate the halfway
            R = normalize( reflect(-L2, N) );    
            diffuse = texel.rgb * fs_in.light_color.rgb * max(dot(N,L2), 0.0);

            //compute the diffuse and specular components for each fragment;
            specular = pow(max(dot(R,V), 0.0),  specular_power) * fs_in.spec;
            color += vec4(fs_in.scalar + diffuse, 1.0) + vec4 (specular,1);


            //Soptlight Calculation/////////////////////////////

            color += calculateSpotLight(fs_in.spotlightPos, N, V);

/*
              vec3 sd = normalize(fs_in.spotLightDir);
              float NdotL = max (dot(sd,L),0.0);
                if ( NdotL > 0.0){
                   L = normalize(fs_in.spotlightPos);
                   R = normalize( reflect(sd,N) );
                   float spotLight = dot(L,V);
                   if (spotLight < .01){
                      V = normalize(fs_in.V);
                      diffuse = max(dot(N,L), 0.0) *  fs_in.diffuse;
                      specular = pow(max(dot(R, V), 0.0), 100.0) *  vec3 (1.0);
                      color += vec4(fs_in.spot + diffuse, 1.0) + vec4 (specular,1);
                  }

              }
*/
              

          }
