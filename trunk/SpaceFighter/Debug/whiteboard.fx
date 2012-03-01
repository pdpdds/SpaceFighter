//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// NPR ASM Effects
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// MetallicCartoon_ASM
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string NPR_ASM_Effects_MetallicCartoon_ASM_Single_Pass_Teapot : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\models\\Teapot.3ds";

//--------------------------------------------------------------//
// Vertex Shader
//--------------------------------------------------------------//
VertexShader NPR_ASM_Effects_MetallicCartoon_ASM_Single_Pass_Vertex_Shader_ = asm
{
   vs.1.1
   dcl_position   v0
   dcl_normal     v3
   dcl_texcoord0  v7
   
   // Output vertex position:
   m4x4 oPos, v0, c5
   
   mov oT0, v7         // Texture Coordinates
   
   sub r0,   c0, v0
   dp3 r0.w, r0, r0
   rsq r0.w, r0.w
   mul oT1,  r0, r0.w  // View Vector
   
   mov oT2, v3         // Normal
   
   sub r0,   c1, v0      // Ray from point to light vLP
   dp3 r0.w, r0, r0
   rsq r0.w, r0.w
   mul oT3,  r0, r0.w
   
   sub r0,   c2, v0      // Ray from point to light vLP
   dp3 r0.w, r0, r0
   rsq r0.w, r0.w
   mul oT4,  r0, r0.w
   
   sub r0,   c3, v0      // Ray from point to light vLP
   dp3 r0.w, r0, r0
   rsq r0.w, r0.w
   mul oT5,  r0, r0.w
   
   

};

//--------------------------------------------------------------//
// Pixel Shader
//--------------------------------------------------------------//
PixelShader NPR_ASM_Effects_MetallicCartoon_ASM_Single_Pass_Pixel_Shader_ = asm
{
   ps.2.0
   
   dcl_2d  s0  // Outline texture
   dcl t0      // Texture coordinates
   dcl t1      // View vector
   dcl t2      // Normal
   dcl t3      // Vector to light one
   dcl t4      // Vector to light two
   dcl t5      // Vector to light three
   
   mov r0, t2
   dp3 r0.w, r0, r0
   rsq r0.w, r0.w
   mul r0, r0, r0.w     // Normalized normal
   
   dp3 r1.x, r0, t1     // N.V
   sub r1.x, c0.z, r1.x // 1 - N.V
   
   mov r1.y, c0.x
   texld r2, r1, s0     // Look up outline term.
   
   dp3 r3.w, t3, t3
   rsq r3.w, r3.w
   mul r3, t3, r3.w     // Normalize light1 vector
   dp3 r1.x, r3, r0     // N.L1
   
   dp3 r3.w, t4, t4
   rsq r3.w, r3.w
   mul r3, t4, r3.w     // Normalize light2 vector
   dp3 r1.y, r3, r0     // N.L2
   
   dp3 r3.w, t5, t5
   rsq r3.w, r3.w
   mul r3, t5, r3.w     // Normalize light3 vector
   dp3 r1.z, r3, r0     // N.L3
   
   mad r1.xyz, r1, c0.yyyy, c0.yyyy // Bias into 0-1 range
   dp3 r3, r1, c0.zzzz              // Sum lighting terms
   
   mul r0, r3, r2       // outline*lighting
   mul r0, c1, r0       // outline*lighting*color
   
   mov oC0, r0
   
   

};

//--------------------------------------------------------------//
// Hatch_ASM
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string NPR_ASM_Effects_Hatch_ASM_Single_Pass_Teapot : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\models\\Teapot.3ds";

//--------------------------------------------------------------//
// Vertex Shader
//--------------------------------------------------------------//
VertexShader NPR_ASM_Effects_Hatch_ASM_Single_Pass_Vertex_Shader_ = asm
{
   // 
   // v0 - Position
   // v1 - Normal
   // v2 - Tex0
   //
   //      SetVertexShaderConstant 0   0
   //      SetVertexShaderConstant 1   1
   //      SetVertexShaderConstant 2   2
   //      SetVertexShaderConstant 3   3
   //      SetVertexShaderConstant 4   4
   //      SetVertexShaderConstant 5   5
   //      SetVertexShaderConstant 6   6
   //      SetVertexShaderConstant 7   7
   //      SetVertexShaderConstant 8   brightness
   //      SetVertexShaderConstant 9   lightPos
   //      SetVertexShaderConstant 10  mvpMatrix
   //      SetVertexShaderConstant 14  mvMatrix
   //      
   vs.1.1
   
   dcl_position   v0
   dcl_normal     v1
   dcl_texcoord   v2
   
   // output the vertex multiplied by the mvp matrix
   m4x4  oPos, v0, c0
   
   // write out the texture coodinate
   mov      oT0, v2
   mov      oT1, v2
   
   // normailize the normal 
   mov      r1, v1
   mov      r1.w, c4.x
   dp3      r2, r1, r1
   rsq      r2, r2
   mul      r1, r1, r2
   
   // compute light vector and normalize
   sub      r2, c9, v0
   dp3      r3, r2, r2
   rsq      r3, r3
   mul      r2, r2, r3
   
   // compute the light factor (n dot l) times six clamp at zero
   dp3      r3, r2, r1
   mul      r3, r3, c8
   
   // seed the blend weights
   mov      r5.x, c5.y
   mov      r5.y, c5.x
   mov      r5.z, c4.w
   mov      r5.w, c4.x
   
   mov      r6.x, c4.z
   mov      r6.y, c4.y
   mov      r6.z, c4.x
   mov      r6.w, c4.x
   
   // sub each weights initial value from the light factor
   sub      r5, r3, r5
   sub      r6, r3, r6
   
   // get rid of everthing less than zero
   max      r5, r5, c4.x      
   
   // flag all weights that are <= 2
   sge      r7, c4.z, r5
   // zero out weights > 2
   mul      r5, r5, r7
   
   // flag all weights that are >= 1
   sge      r7, r5, c4.y
   // subtract all weights that are greater than or equal to one from 2
   mul      r7, r7, c4.z
   sub      r5, r7, r5
   
   // flag all weights that are < 0 and negate
   slt      r7, r5, c4.x
   // flag all spots that are >= 0
   sge      r8, r5, c4.x
   // add the flags
   add      r7, -r7, r8
   // should negate the negatives and leave the positives
   mul      r5, r5, r7
   
   // same as above only on the second set of weights
   max      r6, r6, c4.x      
   sge      r7, c4.z, r6
   mul      r6, r6, r7
   sge      r7, r6, c4.y
   mul      r7, r7, c4.z
   sub      r6, r7, r6
   slt      r7, r6, c4.x
   sge      r8, r6, c4.x
   add      r7, -r7, r8
   mul      r6, r6, r7
   
   // check for total shadow and clamp on the darkest texture
   sge      r8, c4.y, r3
   mov      r7, c4.x
   mov      r7.z, r8.z
   add      r6, r6, r7
   min      r6, r6, c4.y
   
   // write the 123 weights into tex coord 3
   mov      oT2.xyz, r5
   // write the 456 weights into tex coord 4 
   mov      oT3.xyz, r6
   
   

};

//--------------------------------------------------------------//
// Pixel Shader
//--------------------------------------------------------------//
PixelShader NPR_ASM_Effects_Hatch_ASM_Single_Pass_Pixel_Shader_ = asm
{
   ps.1.4
   
   // sample the first texture map
   texld r0, t0
   texld r1, t0
   texld r2, t0
   texld r3, t0
   texld r4, t0
   texld r5, t0
   
   mov_sat r0.g, r1.r 
   mov_sat r0.b, r2.r 
   
   mov_sat r3.g, r4.r 
   mov_sat r3.b, r5.r
   
   phase 
   
   // get the 123 texture weights and place it in register 2
   texcrd   r4.rgb, t2.xyz
   
   // get the 456 texture weights and place it in register 3
   texcrd   r5.rgb, t3.xyz
   
   // dot the reg0 (texture values) with reg2 (texture weights)
   dp3_sat  r0, 1-r0, r4
   
   // dot the reg1 (texture values) with reg3 (texture weights)
   dp3_sat  r3, 1-r3, r5
   
   // add reg 0 and reg1
   add_sat  r0, r0, r3
   
   // compliment and saturate
   mov_sat  r0, 1-r0
   
   // uncomment to draw the texture weights
   //add_sat    r0.xyz, r2, r3
   
   

};

//--------------------------------------------------------------//
// GoochLighting_ASM
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string NPR_ASM_Effects_GoochLighting_ASM_Single_Pass_Teapot : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\models\\Teapot.3ds";

//--------------------------------------------------------------//
// Vertex Shader
//--------------------------------------------------------------//
VertexShader NPR_ASM_Effects_GoochLighting_ASM_Single_Pass_Vertex_Shader_ = asm
{
   ;
   ; v0 - Position
   ; v3 - Normal
   ;
   ;      SetVertexShaderConstant 9 lightPos
   ;      SetVertexShaderConstant 10   mvpMatrix
   ;      SetVertexShaderConstant 14   mvMatrix
   ;      
   vs.1.1
   
   dcl_position   v0
   dcl_normal     v1
   dcl_texcoord   v2
   
   // output the vertex multiplied by the mvp matrix
   m4x4  oPos, v0, c0
   
   // compute the light vector
   //m4x4   r0, v0, c14
   sub      r0, c8, v0
   dp3      r1, r0, r0
   rsq      r1, r1
   mul      r0, r0, r1
   
   // compute the normal
   //m3x3   r1, v1, c14
   mov      r1, v1
   
   // write the normal to tex coord 0
   mov      oT0, r1
   
   // write the light vector to tex coord 1
   mov      oT1, r0
   
   
   

};

//--------------------------------------------------------------//
// Pixel Shader
//--------------------------------------------------------------//
PixelShader NPR_ASM_Effects_GoochLighting_ASM_Single_Pass_Pixel_Shader_ = asm
{
   ps.1.4
   
   
   //float alpha[4] = { 0.3f, 0.3f, 0.3f, 0.3f };
   //float beta[4] = { 0.4f, 0.4f, 0.4f, 0.4f };
   //float blue[4] = { 0.0f, 0.0f, 0.4f, 0.0f };
   //float yellow[4] = { 0.5f, 0.5f, 0.0f, 0.5f };
   //float diffuse[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
   //float one[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
   
   //d3d->SetPixelShaderConstant(0, alpha, 1);     alpha is a constant used in the cool color factor
   //d3d->SetPixelShaderConstant(1, beta, 1);      beta is a constant used in the warm color factor
   //d3d->SetPixelShaderConstant(2, yellow, 1); yellow is used with the warm factor
   //d3d->SetPixelShaderConstant(3, blue, 1);      blue is used with the cool factor
   //d3d->SetPixelShaderConstant(4, diffuse, 1);   diffuse is the actual diffuse color of the object
   //d3d->SetPixelShaderConstant(5, one, 1);    one ie one
   
   
   // get the normal and place it in register 0
   texcrd   r0.xyz, t0
   
   // get the light vector and put it in register 1
   texcrd   r1.xyz, t1
   
   // compute n dot l and place it in register 3
   dp3      r3, r0, r1
   
   // normalize the n dot l range
   add_d2_sat   r3, r3, c5
   
   // compute the cool factor
   mul_sat r0, c4, c0 
   add_sat r0, r0, c2 
   mul_sat r0, r0, r3
   
   // compute the warm factor
   mul_sat r1, c4, c1 
   add_sat r1, r1, c3 
   mad_sat r0, r1, 1-r3, r0
   

};

//--------------------------------------------------------------//
// ToonWithDynamicSpecular_ASM
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string NPR_ASM_Effects_ToonWithDynamicSpecular_ASM_Single_Pass_Teapot : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\models\\Teapot.3ds";

//--------------------------------------------------------------//
// Vertex Shader
//--------------------------------------------------------------//
VertexShader NPR_ASM_Effects_ToonWithDynamicSpecular_ASM_Single_Pass_Vertex_Shader_ = asm
{
   ;
   ; v0 - Position
   ; v3 - Normal
   ;
   ;      SetVertexShaderConstant 9 lightPos
   ;      SetVertexShaderConstant 10   mvpMatrix
   ;      SetVertexShaderConstant 14   mvMatrix
   ;      
   vs.1.1
   
   dcl_position   v0
   dcl_normal     v1
   dcl_texcoord   v2
   
   // output the vertex multiplied by the mvp matrix
   m4x4  oPos, v0, c0
   
   // compute the normal in eye space
   m3x3  r0, v1, c4
   mov      oT0.xyz, r0 // write the normal to tex coord 0
   
   // compute the light vector
   sub      r0, c8, v0
   dp3      r1, r0, r0
   rsq      r1, r1
   mul      r0, r0, r1
   m3x3  r1, r0, c4 // transform the light vector into eye space
   mov      oT1, r1 // write the light vector to tex coord 1
   
   // compute half vector
   m4x4  r0, v0, c4 // transform the vertex position into eye space 
   dp3      r3, r0, r0 // normalize to get the view vector
   rsq      r3, r3
   mul      r0, r0, r3
   add      r0, r1, -r0 // add the light vector and the view vector = half angle
   dp3      r3, r0, r0 // normalize the half angle vector
   rsq      r3, r3
   mul      r0, r0, r3
   
   // write the half angle vector to tex coord 2
   mov      oT2, r0
   
   
   
   

};

//--------------------------------------------------------------//
// Pixel Shader
//--------------------------------------------------------------//
PixelShader NPR_ASM_Effects_ToonWithDynamicSpecular_ASM_Single_Pass_Pixel_Shader_ = asm
{
   ps.1.4
   
   //def c0, 0.1f, 0.1f, 0.1f, 0.1f // falloff 1
   //def c1, 0.88f, 0.88f, 0.88f, 0.88f // falloff 2
   //def c2, 0.4f, 0.4f, 0.4f, 1.0f // ambient
   //def c3, 0.6f, 0.6f, 0.6f, 1.0f // diffuse
   //def c4, 0.8f, 0.8f, 0.8f, 1.0f // specular
   
   // get the normal and place it in register 0
   texcrd   r0.xyz, t0
   
   // get the light vector and put it in register 1
   texcrd   r1.xyz, t1
   
   // get the half angle vector and put it in register 2
   texcrd   r2.xyz, t2
   
   // compute n dot l and place it in register 3
   dp3      r3, r0, r1
   
   // compute n dot h and place it in register 3
   dp3      r4, r0, r2
   
   // subtract falloff 1 from the n dot l computation
   sub      r3, r3, c0
   
   // check if n dot l is greater than zero---if yes use diffuse color otherwise use the ambient
   cmp_sat r0, r3, c3, c2
   
   // subtract falloff 2 from the n dot half angle computation
   sub r4, r4, c1
   
   // check if n dot h is greater than zero---if yes use specular color otherwise use ambient
   cmp_sat r0, r4, c4, r0
   
   

};

//--------------------------------------------------------------//
// ToonWithoutSpecular_ASM
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string NPR_ASM_Effects_ToonWithoutSpecular_ASM_Single_Pass_Teapot : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\models\\Teapot.3ds";

//--------------------------------------------------------------//
// Vertex Shader
//--------------------------------------------------------------//
VertexShader NPR_ASM_Effects_ToonWithoutSpecular_ASM_Single_Pass_Vertex_Shader_ = asm
{
   ;
   ; v0 - Position
   ; v3 - Normal
   ;
   ;      SetVertexShaderConstant 9 lightPos
   ;      SetVertexShaderConstant 10   mvpMatrix
   ;      SetVertexShaderConstant 14   mvMatrix
   ;      
   vs.1.1
   
   dcl_position   v0
   dcl_normal     v1
   dcl_texcoord   v2
   
   // output the vertex multiplied by the mvp matrix
   m4x4  oPos, v0, c0
   
   // compute the normal in eye space
   m3x3  r0, v1, c4
   mov      oT0.xyz, r0 // write the normal to tex coord 0
   
   // compute the light vector
   sub      r0, c8, v0
   dp3      r1, r0, r0
   rsq      r1, r1
   mul      r0, r0, r1
   m3x3  r1, r0, c4 // transform the light vector into eye space
   mov      oT1, r1 // write the light vector to tex coord 1
   
   // compute half vector
   m4x4  r0, v0, c4 // transform the vertex position into eye space 
   dp3      r3, r0, r0 // normalize to get the view vector
   rsq      r3, r3
   mul      r0, r0, r3
   add      r0, r1, -r0 // add the light vector and the view vector = half angle
   dp3      r3, r0, r0 // normalize the half angle vector
   rsq      r3, r3
   mul      r0, r0, r3
   
   // write the half angle vector to tex coord 2
   mov      oT2, r0
   
   
   
   

};

//--------------------------------------------------------------//
// Pixel Shader
//--------------------------------------------------------------//
PixelShader NPR_ASM_Effects_ToonWithoutSpecular_ASM_Single_Pass_Pixel_Shader_ = asm
{
   ps.1.4
   
   //def c0, 0.1f, 0.1f, 0.1f, 0.1f // falloff 1
   //def c1, 0.8f, 0.8f, 0.8f, 0.8f // falloff 2
   //def c2, 0.4f, 0.4f, 0.4f, 1.0f // dark
   //def c3, 0.6f, 0.6f, 0.6f, 1.0f // middle
   //def c4, 0.8f, 0.8f, 0.8f, 1.0f // light
   
   // get the normal and place it in register 0
   texcrd   r0.xyz, t0
   
   // get the light vector and put it in register 1
   texcrd   r1.xyz, t1
   
   // compute n dot l and place it in register 3
   dp3      r3, r0, r1
   
   // subtract falloff 1 from the n dot l computation
   sub      r4, r3, c0
   
   // check if n dot l is greater than zero---if yes use middle color otherwise use the dark color
   cmp_sat r0, r4, c3, c2
   
   // subtract falloff 2 from the n dot half angle computation
   sub r4, r3, c1
   
   // check if n dot h is greater than zero---if yes use bright color otherwise use whats there
   cmp_sat r0, r4, c4, r0
   
   

};

//--------------------------------------------------------------//
// NPR
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Silhouette
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string NPR_Silhouette_Pass_0_Teapot : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\models\\Teapot.3ds";

texture RT_Tex : RenderColorTarget
<
   float2 ViewportRatio={1.0,1.0};
   string Format="D3DFMT_A8R8G8B8";
   float  ClearDepth=1.000000;
   int    ClearColor=0;
>;
float4x4 view_proj_matrix : ViewProjection;
struct VS_OUTPUT
{
   float4 Pos: POSITION;
};

VS_OUTPUT NPR_Silhouette_Pass_0_Vertex_Shader_vs_main( float4 Pos: POSITION )
{
   VS_OUTPUT Out = (VS_OUTPUT) 0; 
   Out.Pos = mul( view_proj_matrix, Pos );
   return Out;
}

float4 NPR_Silhouette_Pass_0_Pixel_Shader_ps_main( float4 Diff: COLOR0 ) : COLOR
{
    float4 c; 
    c[0] = 1.0;
    c[1] = 1.0;
    c[2] = 1.0;
    c[3] = 1.0;

    return c;
}

//--------------------------------------------------------------//
// Pass 1
//--------------------------------------------------------------//
string NPR_Silhouette_Pass_1_ScreenAlignedQuad : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

float4x4 NPR_Silhouette_Pass_1_Vertex_Shader_view_proj_matrix;
struct VS_INPUT
{
   float4 Pos      : POSITION;
   float2 TexCoord : TEXCOORD0;
};

struct NPR_Silhouette_Pass_1_Vertex_Shader_VS_OUTPUT
{
   float4 Pos      : POSITION;
   float2 TexCoord : TEXCOORD0;
};

NPR_Silhouette_Pass_1_Vertex_Shader_VS_OUTPUT NPR_Silhouette_Pass_1_Vertex_Shader_vs_main( VS_INPUT In )
{
   NPR_Silhouette_Pass_1_Vertex_Shader_VS_OUTPUT Out; 

   In.Pos.xy = sign(In.Pos.xy);
   Out.Pos       = float4(In.Pos.xy, 0.0, 1.0);
   // Image-space
   Out.TexCoord.x = 0.5 * (1 + In.Pos.x);
   Out.TexCoord.y = 0.5 * (1 - In.Pos.y);

   return Out;
}

sampler RT = sampler_state
{
   Texture = (RT_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
};
// One pixel offset
const float off = 1.0 / 512.0;


struct PS_INPUT
{
   float2 TexCoord : TEXCOORD0;
};

struct PS_OUTPUT
{
   float4 Color    : COLOR0;
};


PS_OUTPUT NPR_Silhouette_Pass_1_Pixel_Shader_ps_main( PS_INPUT In)
{
   PS_OUTPUT Out;

   // Sample neighbor pixels
   float s00 = tex2D(RT, In.TexCoord + float2(-off, -off)).r;
   float s01 = tex2D(RT, In.TexCoord + float2( 0,   -off)).r;
   float s02 = tex2D(RT, In.TexCoord + float2( off, -off)).r;

   float s10 = tex2D(RT, In.TexCoord + float2(-off,  0)).r;
   float s12 = tex2D(RT, In.TexCoord + float2( off,  0)).r;

   float s20 = tex2D(RT, In.TexCoord + float2(-off,  off)).r;
   float s21 = tex2D(RT, In.TexCoord + float2( 0,    off)).r;
   float s22 = tex2D(RT, In.TexCoord + float2( off,  off)).r;

   // Sobel filter in X direction
   float sobelX = s00 + 2 * s10 + s20 - s02 - 2 * s12 - s22;
   // Sobel filter in Y direction
   float sobelY = s00 + 2 * s01 + s02 - s20 - 2 * s21 - s22;

   // Find edge, skip sqrt() to improve performance ...
   float edgeSqr = (sobelX * sobelX + sobelY * sobelY);

   // ... and threshold against a squared value instead.
   Out.Color = 1.0-(edgeSqr > 0.07 * 0.07);
   return Out;
}


//--------------------------------------------------------------//
// ToonWithDynamicSpecular
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string NPR_ToonWithDynamicSpecular_Single_Pass_Teapot : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\models\\Teapot.3ds";

float4x4 NPR_ToonWithDynamicSpecular_Single_Pass_Vertex_Shader_view_proj_matrix : ViewProjection;
float4x4 view_matrix : View;
float4 lightDir
<
   string UIName = "lightDir";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 0.00, 0.00, 1.00, 0.00 );
struct NPR_ToonWithDynamicSpecular_Single_Pass_Vertex_Shader_VS_INPUT
{
   float4 Pos    : POSITION0;
   float3 Normal : NORMAL0;
};

struct NPR_ToonWithDynamicSpecular_Single_Pass_Vertex_Shader_VS_OUTPUT
{
   float4 Pos      : POSITION0;
   float2 TexCoord : TEXCOORD0;
};

NPR_ToonWithDynamicSpecular_Single_Pass_Vertex_Shader_VS_OUTPUT NPR_ToonWithDynamicSpecular_Single_Pass_Vertex_Shader_vs_main( NPR_ToonWithDynamicSpecular_Single_Pass_Vertex_Shader_VS_INPUT In )
{
   NPR_ToonWithDynamicSpecular_Single_Pass_Vertex_Shader_VS_OUTPUT Out; 

   Out.Pos = mul( NPR_ToonWithDynamicSpecular_Single_Pass_Vertex_Shader_view_proj_matrix, In.Pos );

   float3 posW    = mul( view_matrix, In.Pos );
   float3 normalW = mul( (float3x3)view_matrix, In.Normal );

   float diffuse = max(0,dot(-lightDir,normalW));
   Out.TexCoord.x = diffuse;
   Out.TexCoord.y = 0.0f;

   return Out;
}

texture ToonShaderTexture_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Textures\\Toon.bmp";
>;
sampler ToonShaderTexture = sampler_state
{
   Texture = (ToonShaderTexture_Tex);
};
struct NPR_ToonWithDynamicSpecular_Single_Pass_Pixel_Shader_PS_INPUT
{
   float2 TexCoord : TEXCOORD0;
};

struct NPR_ToonWithDynamicSpecular_Single_Pass_Pixel_Shader_PS_OUTPUT
{
   float4 Color    : COLOR0;
};

NPR_ToonWithDynamicSpecular_Single_Pass_Pixel_Shader_PS_OUTPUT NPR_ToonWithDynamicSpecular_Single_Pass_Pixel_Shader_ps_main( NPR_ToonWithDynamicSpecular_Single_Pass_Pixel_Shader_PS_INPUT In )
{
    NPR_ToonWithDynamicSpecular_Single_Pass_Pixel_Shader_PS_OUTPUT Out;
 
    Out.Color = tex2D(ToonShaderTexture,In.TexCoord);
    
    return Out;
}

//--------------------------------------------------------------//
// ToonWithSilhouetteRendering
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// DrawObjToon
//--------------------------------------------------------------//
string NPR_ToonWithSilhouetteRendering_DrawObjToon_Teapot : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\models\\Teapot.3ds";

texture RTToonObj_Tex : RenderColorTarget
<
   float2 ViewportRatio={1.0,1.0};
   string Format="D3DFMT_A8R8G8B8";
   float  ClearDepth=1.000000;
   int    ClearColor=50688;
>;
float4x4 NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_view_proj_matrix : ViewProjection;
float4x4 NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_view_matrix : View;
float4 NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_lightDir
<
   string UIName = "NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_lightDir";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 0.00, 0.00, 1.00, 0.00 );
struct NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_VS_INPUT
{
   float4 Pos    : POSITION0;
   float3 Normal : NORMAL0;
};

struct NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_VS_OUTPUT
{
   float4 Pos      : POSITION0;
   float2 TexCoord : TEXCOORD0;
};

NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_VS_OUTPUT NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_vs_main( NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_VS_INPUT In )
{
   NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_VS_OUTPUT Out; 

   Out.Pos = mul( NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_view_proj_matrix, In.Pos );

   float3 posW    = mul( NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_view_matrix, In.Pos );
   float3 normalW = mul( (float3x3)NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_view_matrix, In.Normal );

   float diffuse = max(0,dot(-NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_lightDir,normalW));
   Out.TexCoord.x = diffuse;
   Out.TexCoord.y = 0.0f;

   return Out;
}

sampler NPR_ToonWithSilhouetteRendering_DrawObjToon_Pixel_Shader_ToonShaderTexture = sampler_state
{
   Texture = (ToonShaderTexture_Tex);
};
struct NPR_ToonWithSilhouetteRendering_DrawObjToon_Pixel_Shader_PS_INPUT
{
   float2 TexCoord : TEXCOORD0;
};

struct NPR_ToonWithSilhouetteRendering_DrawObjToon_Pixel_Shader_PS_OUTPUT
{
   float4 Color    : COLOR0;
};

NPR_ToonWithSilhouetteRendering_DrawObjToon_Pixel_Shader_PS_OUTPUT NPR_ToonWithSilhouetteRendering_DrawObjToon_Pixel_Shader_ps_main( NPR_ToonWithSilhouetteRendering_DrawObjToon_Pixel_Shader_PS_INPUT In )
{
    NPR_ToonWithSilhouetteRendering_DrawObjToon_Pixel_Shader_PS_OUTPUT Out;
 
    Out.Color.xyz = tex2D(NPR_ToonWithSilhouetteRendering_DrawObjToon_Pixel_Shader_ToonShaderTexture,In.TexCoord).xyz;
    Out.Color.w   = 1.0;
    
    return Out;
}

//--------------------------------------------------------------//
// Outline
//--------------------------------------------------------------//
string NPR_ToonWithSilhouetteRendering_Outline_ScreenAlignedQuad : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

texture RTOutlineResult_Tex : RenderColorTarget
<
   float2 RenderTargetDimensions = {512,512};
   string Format="D3DFMT_A8R8G8B8";
   float  ClearDepth=0.000000;
   int    ClearColor=0;
>;
float4x4 NPR_ToonWithSilhouetteRendering_Outline_Vertex_Shader_view_proj_matrix;
struct NPR_ToonWithSilhouetteRendering_Outline_Vertex_Shader_VS_INPUT
{
   float4 Pos      : POSITION;
   float2 TexCoord : TEXCOORD0;
};

struct NPR_ToonWithSilhouetteRendering_Outline_Vertex_Shader_VS_OUTPUT
{
   float4 Pos      : POSITION;
   float2 TexCoord : TEXCOORD0;
};

NPR_ToonWithSilhouetteRendering_Outline_Vertex_Shader_VS_OUTPUT NPR_ToonWithSilhouetteRendering_Outline_Vertex_Shader_vs_main( NPR_ToonWithSilhouetteRendering_Outline_Vertex_Shader_VS_INPUT In )
{
   NPR_ToonWithSilhouetteRendering_Outline_Vertex_Shader_VS_OUTPUT Out; 

   In.Pos.xy = sign(In.Pos.xy);
   Out.Pos       = float4(In.Pos.xy, 0.0, 1.0);
   // Image-space
   Out.TexCoord.x = 0.5 * (1 + In.Pos.x);
   Out.TexCoord.y = 0.5 * (1 - In.Pos.y);

   return Out;
}

sampler OutlineInput = sampler_state
{
   Texture = (RTToonObj_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
};

// One pixel offset
const float NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_off = 1.0 / 512.0;


struct NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_PS_INPUT
{
   float2 TexCoord : TEXCOORD0;
};

struct NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_PS_OUTPUT
{
   float4 Color    : COLOR0;
};


NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_PS_OUTPUT NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_ps_main( NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_PS_INPUT In)
{
   NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_PS_OUTPUT Out;

   // Sample neighbor pixels
   float s00 = tex2D(OutlineInput, In.TexCoord + float2(-NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_off, -NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_off)).w;
   float s01 = tex2D(OutlineInput, In.TexCoord + float2( 0,   -NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_off)).w;
   float s02 = tex2D(OutlineInput, In.TexCoord + float2( NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_off, -NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_off)).w;

   float s10 = tex2D(OutlineInput, In.TexCoord + float2(-NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_off,  0)).w;
   float s12 = tex2D(OutlineInput, In.TexCoord + float2( NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_off,  0)).w;

   float s20 = tex2D(OutlineInput, In.TexCoord + float2(-NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_off,  NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_off)).w;
   float s21 = tex2D(OutlineInput, In.TexCoord + float2( 0,    NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_off)).w;
   float s22 = tex2D(OutlineInput, In.TexCoord + float2( NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_off,  NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_off)).w;

   // Sobel filter in X direction
   float sobelX = s00 + 2 * s10 + s20 - s02 - 2 * s12 - s22;
   // Sobel filter in Y direction
   float sobelY = s00 + 2 * s01 + s02 - s20 - 2 * s21 - s22;

   // Find edge, skip sqrt() to improve performance ...
   float edgeSqr = (sobelX * sobelX + sobelY * sobelY);

   // ... and threshold against a squared value instead.
   Out.Color.xyz = (edgeSqr > 0.07 * 0.07);
   Out.Color.w   = 1.0;
   return Out;
}


//--------------------------------------------------------------//
// Composite
//--------------------------------------------------------------//
string NPR_ToonWithSilhouetteRendering_Composite_ScreenAlignedQuad : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

float4x4 NPR_ToonWithSilhouetteRendering_Composite_Vertex_Shader_view_proj_matrix;
struct NPR_ToonWithSilhouetteRendering_Composite_Vertex_Shader_VS_INPUT
{
   float4 Pos      : POSITION;
};

struct NPR_ToonWithSilhouetteRendering_Composite_Vertex_Shader_VS_OUTPUT
{
   float4 Pos      : POSITION;
   float2 TexCoord : TEXCOORD0;
};

NPR_ToonWithSilhouetteRendering_Composite_Vertex_Shader_VS_OUTPUT NPR_ToonWithSilhouetteRendering_Composite_Vertex_Shader_vs_main( NPR_ToonWithSilhouetteRendering_Composite_Vertex_Shader_VS_INPUT In )
{
   NPR_ToonWithSilhouetteRendering_Composite_Vertex_Shader_VS_OUTPUT Out; 

   In.Pos.xy = sign(In.Pos.xy);
   Out.Pos       = float4(In.Pos.xy, 0.0, 1.0);
   // Image-space
   Out.TexCoord.x = 0.5 * (1 + In.Pos.x);
   Out.TexCoord.y = 0.5 * (1 - In.Pos.y);

   return Out;
}

sampler ObjInput = sampler_state
{
   Texture = (RTToonObj_Tex);
};
sampler NPR_ToonWithSilhouetteRendering_Composite_Pixel_Shader_OutlineInput = sampler_state
{
   Texture = (RTOutlineResult_Tex);
};
struct NPR_ToonWithSilhouetteRendering_Composite_Pixel_Shader_PS_INPUT
{
   float2 TexCoord : TEXCOORD0;
};

struct NPR_ToonWithSilhouetteRendering_Composite_Pixel_Shader_PS_OUTPUT
{
   float4 Color    : COLOR0;
};

NPR_ToonWithSilhouetteRendering_Composite_Pixel_Shader_PS_OUTPUT NPR_ToonWithSilhouetteRendering_Composite_Pixel_Shader_ps_main( NPR_ToonWithSilhouetteRendering_Composite_Pixel_Shader_PS_INPUT In )
{
    NPR_ToonWithSilhouetteRendering_Composite_Pixel_Shader_PS_OUTPUT Out;
 
    float4 objColor     = tex2D(ObjInput,In.TexCoord);
    float4 outLineColor = tex2D(NPR_ToonWithSilhouetteRendering_Composite_Pixel_Shader_OutlineInput,In.TexCoord);

    if (outLineColor.x > 0.0)
       Out.Color = 0.0;
    else
       Out.Color = objColor;
    
    return Out;
}


//--------------------------------------------------------------//
// Hatch
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string NPR_Hatch_Single_Pass_Teapot : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\models\\Teapot.3ds";

float4 NPR_Hatch_Single_Pass_Vertex_Shader_lightDir
<
   string UIName = "NPR_Hatch_Single_Pass_Vertex_Shader_lightDir";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 0.00, 0.00, 1.00, 0.00 );
float4x4 NPR_Hatch_Single_Pass_Vertex_Shader_view_matrix : View;
float4x4 NPR_Hatch_Single_Pass_Vertex_Shader_view_proj_matrix : ViewProjection;
struct NPR_Hatch_Single_Pass_Vertex_Shader_VS_INPUT
{
   float4 Pos      : POSITION0;
   float3 Normal   : NORMAL0;
   float2 TexCoord : TEXCOORD0;
};

struct NPR_Hatch_Single_Pass_Vertex_Shader_VS_OUTPUT
{
   float4 Pos           : POSITION0;
   float2 TexCoord      : TEXCOORD0;
   float3 HatchWeights0 : TEXCOORD1;
   float3 HatchWeights1 : TEXCOORD2;
};

NPR_Hatch_Single_Pass_Vertex_Shader_VS_OUTPUT NPR_Hatch_Single_Pass_Vertex_Shader_vs_main( NPR_Hatch_Single_Pass_Vertex_Shader_VS_INPUT In )
{
   NPR_Hatch_Single_Pass_Vertex_Shader_VS_OUTPUT Out; 

   Out.Pos = mul( NPR_Hatch_Single_Pass_Vertex_Shader_view_proj_matrix, In.Pos );
   Out.TexCoord = In.TexCoord;

   float3 posW    = mul( NPR_Hatch_Single_Pass_Vertex_Shader_view_matrix, In.Pos );
   float3 normalW = normalize(mul( (float3x3)NPR_Hatch_Single_Pass_Vertex_Shader_view_matrix, In.Normal ));

   float  diffuse = min(1.0,max(0,dot(-NPR_Hatch_Single_Pass_Vertex_Shader_lightDir,normalW)));
   diffuse = diffuse * diffuse;
   diffuse = diffuse * diffuse;

   float  hatchFactor = diffuse * 6.0;
   float3 weight0 = 0.0;
   float3 weight1 = 0.0;

   if (hatchFactor>5.0)
   {
      weight0.x = 1.0;
   } // End if

   else if (hatchFactor>4.0)
   {
      weight0.x = 1.0 - (5.0 - hatchFactor);
      weight0.y = 1.0 - weight0.x;
   } // End else if

   else if (hatchFactor>3.0)
   {
      weight0.y = 1.0 - (4.0 - hatchFactor);
      weight0.z = 1.0 - weight0.y;
   } // End else if

   else if (hatchFactor>2.0)
   {
      weight0.z = 1.0 - (3.0 - hatchFactor);
      weight1.x = 1.0 - weight0.z;
   } // End else if

   else if (hatchFactor>1.0)
   {
      weight1.x = 1.0 - (2.0 - hatchFactor);
      weight1.y = 1.0 - weight1.x;
   } // End else if

   else if (hatchFactor>0.0)
   {
      weight1.y = 1.0 - (1.0 - hatchFactor);
      weight1.z = 1.0 - weight1.y;
   } // End else if

   Out.HatchWeights0 = weight0;
   Out.HatchWeights1 = weight1;

   return Out;
}


float4 MiscParam;
texture hatch0_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Textures\\Hatch0.dds";
>;
sampler Hatch0 = sampler_state
{
   Texture = (hatch0_Tex);
   MIPFILTER = LINEAR;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
};
texture hatch1_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Textures\\Hatch1.dds";
>;
sampler Hatch1 = sampler_state
{
   Texture = (hatch1_Tex);
   MIPFILTER = LINEAR;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
texture hatch2_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Textures\\Hatch2.dds";
>;
sampler Hatch2 = sampler_state
{
   Texture = (hatch2_Tex);
   MIPFILTER = LINEAR;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
texture hatch3_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Textures\\Hatch3.dds";
>;
sampler Hatch3 = sampler_state
{
   Texture = (hatch3_Tex);
   MIPFILTER = LINEAR;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
texture hatch4_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Textures\\Hatch4.dds";
>;
sampler Hatch4 = sampler_state
{
   Texture = (hatch4_Tex);
   MIPFILTER = LINEAR;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
texture hatch5_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Textures\\Hatch5.dds";
>;
sampler Hatch5 = sampler_state
{
   Texture = (hatch5_Tex);
   MIPFILTER = LINEAR;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
struct NPR_Hatch_Single_Pass_Pixel_Shader_PS_INPUT
{
   float2 TexCoord      : TEXCOORD0;
   float3 HatchWeights0 : TEXCOORD1;
   float3 HatchWeights1 : TEXCOORD2;
};

struct NPR_Hatch_Single_Pass_Pixel_Shader_PS_OUTPUT
{
   float4 Color    : COLOR0;
};

NPR_Hatch_Single_Pass_Pixel_Shader_PS_OUTPUT NPR_Hatch_Single_Pass_Pixel_Shader_ps_main( NPR_Hatch_Single_Pass_Pixel_Shader_PS_INPUT In )
{
    NPR_Hatch_Single_Pass_Pixel_Shader_PS_OUTPUT Out;
 
    float4 hatchTex0 = tex2D(Hatch0,In.TexCoord) * In.HatchWeights0.x;
    float4 hatchTex1 = tex2D(Hatch1,In.TexCoord) * In.HatchWeights0.y;
    float4 hatchTex2 = tex2D(Hatch2,In.TexCoord) * In.HatchWeights0.z;
    float4 hatchTex3 = tex2D(Hatch3,In.TexCoord) * In.HatchWeights1.x;
    float4 hatchTex4 = tex2D(Hatch4,In.TexCoord) * In.HatchWeights1.y;
    float4 hatchTex5 = tex2D(Hatch5,In.TexCoord) * In.HatchWeights1.z;
    
    float4 hatchColor = hatchTex0 +
                        hatchTex1 +
                        hatchTex2 +
                        hatchTex3 +
                        hatchTex4 +
                        hatchTex5;

    Out.Color = hatchColor;

    return Out;
}


//--------------------------------------------------------------//
// HatchWithSilhouetteRendering
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// DrawObjHatch
//--------------------------------------------------------------//
string NPR_HatchWithSilhouetteRendering_DrawObjHatch_Teapot : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\models\\Teapot.3ds";

texture RTHatchObj_Tex : RenderColorTarget
<
   float2 ViewportRatio={1.0,1.0};
   string Format="D3DFMT_A8R8G8B8";
   float  ClearDepth=1.000000;
   int    ClearColor=16777215;
>;
float4 NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_lightDir
<
   string UIName = "NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_lightDir";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 0.00, 0.00, 1.00, 0.00 );
float4x4 NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_view_matrix : View;
float4x4 NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_view_proj_matrix : ViewProjection;
struct NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_VS_INPUT
{
   float4 Pos      : POSITION0;
   float3 Normal   : NORMAL0;
   float2 TexCoord : TEXCOORD0;
};

struct NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_VS_OUTPUT
{
   float4 Pos           : POSITION0;
   float2 TexCoord      : TEXCOORD0;
   float3 HatchWeights0 : TEXCOORD1;
   float3 HatchWeights1 : TEXCOORD2;
};

NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_VS_OUTPUT NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_vs_main( NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_VS_INPUT In )
{
   NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_VS_OUTPUT Out; 

   Out.Pos = mul( NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_view_proj_matrix, In.Pos );
   Out.TexCoord = In.TexCoord;

   float3 posW    = mul( NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_view_matrix, In.Pos );
   float3 normalW = normalize(mul( (float3x3)NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_view_matrix, In.Normal ));

   float  diffuse = min(1.0,max(0,dot(-NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_lightDir,normalW)));
   diffuse = diffuse * diffuse;
   diffuse = diffuse * diffuse;

   float  hatchFactor = diffuse * 6.0;
   float3 weight0 = 0.0;
   float3 weight1 = 0.0;

   if (hatchFactor>5.0)
   {
      weight0.x = 1.0;
   } // End if

   else if (hatchFactor>4.0)
   {
      weight0.x = 1.0 - (5.0 - hatchFactor);
      weight0.y = 1.0 - weight0.x;
   } // End else if

   else if (hatchFactor>3.0)
   {
      weight0.y = 1.0 - (4.0 - hatchFactor);
      weight0.z = 1.0 - weight0.y;
   } // End else if

   else if (hatchFactor>2.0)
   {
      weight0.z = 1.0 - (3.0 - hatchFactor);
      weight1.x = 1.0 - weight0.z;
   } // End else if

   else if (hatchFactor>1.0)
   {
      weight1.x = 1.0 - (2.0 - hatchFactor);
      weight1.y = 1.0 - weight1.x;
   } // End else if

   else if (hatchFactor>0.0)
   {
      weight1.y = 1.0 - (1.0 - hatchFactor);
      weight1.z = 1.0 - weight1.y;
   } // End else if

   Out.HatchWeights0 = weight0;
   Out.HatchWeights1 = weight1;

   return Out;
}


sampler NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_Hatch0 = sampler_state
{
   Texture = (hatch0_Tex);
   MIPFILTER = LINEAR;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
};
sampler NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_Hatch1 = sampler_state
{
   Texture = (hatch1_Tex);
   MIPFILTER = LINEAR;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
sampler NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_Hatch2 = sampler_state
{
   Texture = (hatch2_Tex);
   MIPFILTER = LINEAR;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
sampler NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_Hatch3 = sampler_state
{
   Texture = (hatch3_Tex);
   MIPFILTER = LINEAR;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
sampler NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_Hatch4 = sampler_state
{
   Texture = (hatch4_Tex);
   MIPFILTER = LINEAR;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
sampler NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_Hatch5 = sampler_state
{
   Texture = (hatch5_Tex);
   MIPFILTER = LINEAR;
   MINFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
struct NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_PS_INPUT
{
   float2 TexCoord      : TEXCOORD0;
   float3 HatchWeights0 : TEXCOORD1;
   float3 HatchWeights1 : TEXCOORD2;
};

struct NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_PS_OUTPUT
{
   float4 Color    : COLOR0;
};

NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_PS_OUTPUT NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_ps_main( NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_PS_INPUT In )
{
    NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_PS_OUTPUT Out;
 
    float4 hatchTex0 = tex2D(NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_Hatch0,In.TexCoord) * In.HatchWeights0.x;
    float4 hatchTex1 = tex2D(NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_Hatch1,In.TexCoord) * In.HatchWeights0.y;
    float4 hatchTex2 = tex2D(NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_Hatch2,In.TexCoord) * In.HatchWeights0.z;
    float4 hatchTex3 = tex2D(NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_Hatch3,In.TexCoord) * In.HatchWeights1.x;
    float4 hatchTex4 = tex2D(NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_Hatch4,In.TexCoord) * In.HatchWeights1.y;
    float4 hatchTex5 = tex2D(NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_Hatch5,In.TexCoord) * In.HatchWeights1.z;
    
    float4 hatchColor = hatchTex0 +
                        hatchTex1 +
                        hatchTex2 +
                        hatchTex3 +
                        hatchTex4 +
                        hatchTex5;

    Out.Color.xyz = hatchColor;
    Out.Color.w   = 1.0;

    return Out;
}


//--------------------------------------------------------------//
// Outline
//--------------------------------------------------------------//
string NPR_HatchWithSilhouetteRendering_Outline_ScreenAlignedQuad : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

texture NPR_HatchWithSilhouetteRendering_Outline_RTOutlineResult : RenderColorTarget
<
   float2 RenderTargetDimensions = {512,512};
   string Format="D3DFMT_A8R8G8B8";
   float  ClearDepth=0.000000;
   int    ClearColor=0;
>;
float4x4 NPR_HatchWithSilhouetteRendering_Outline_Vertex_Shader_view_proj_matrix;
//----------------------------------------------------------------------------------//
// Hatch & Silhouette Style Shader                                                  //   
//                                          //
// Author: Toshiaki Tsuji                               //
//                                          //
// (C) ATI Research, 2003                              //
//                                                                                  //
// - This Effect combines hathing and silhouette.                                   //   
//----------------------------------------------------------------------------------//

struct NPR_HatchWithSilhouetteRendering_Outline_Vertex_Shader_VS_INPUT
{
   float4 Pos      : POSITION;
   float2 TexCoord : TEXCOORD0;
};

struct NPR_HatchWithSilhouetteRendering_Outline_Vertex_Shader_VS_OUTPUT
{
   float4 Pos      : POSITION;
   float2 TexCoord : TEXCOORD0;
};

NPR_HatchWithSilhouetteRendering_Outline_Vertex_Shader_VS_OUTPUT NPR_HatchWithSilhouetteRendering_Outline_Vertex_Shader_vs_main( NPR_HatchWithSilhouetteRendering_Outline_Vertex_Shader_VS_INPUT In )
{
   NPR_HatchWithSilhouetteRendering_Outline_Vertex_Shader_VS_OUTPUT Out; 

   In.Pos.xy = sign(In.Pos.xy);
   Out.Pos       = float4(In.Pos.xy, 0.0, 1.0);
   // Image-space
   Out.TexCoord.x = 0.5 * (1 + In.Pos.x);
   Out.TexCoord.y = 0.5 * (1 - In.Pos.y);

   return Out;
}

sampler NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_OutlineInput = sampler_state
{
   Texture = (RTHatchObj_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
};
// One pixel offset
const float NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_off = 1.0 / 512.0;


struct NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_PS_INPUT
{
   float2 TexCoord : TEXCOORD0;
};

struct NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_PS_OUTPUT
{
   float4 Color    : COLOR0;
};


NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_PS_OUTPUT NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_ps_main( NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_PS_INPUT In)
{
   NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_PS_OUTPUT Out;

   // Sample neighbor pixels
   float s00 = tex2D(NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_OutlineInput, In.TexCoord + float2(-NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_off, -NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_off)).w;
   float s01 = tex2D(NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_OutlineInput, In.TexCoord + float2( 0,   -NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_off)).w;
   float s02 = tex2D(NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_OutlineInput, In.TexCoord + float2( NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_off, -NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_off)).w;

   float s10 = tex2D(NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_OutlineInput, In.TexCoord + float2(-NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_off,  0)).w;
   float s12 = tex2D(NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_OutlineInput, In.TexCoord + float2( NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_off,  0)).w;

   float s20 = tex2D(NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_OutlineInput, In.TexCoord + float2(-NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_off,  NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_off)).w;
   float s21 = tex2D(NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_OutlineInput, In.TexCoord + float2( 0,    NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_off)).w;
   float s22 = tex2D(NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_OutlineInput, In.TexCoord + float2( NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_off,  NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_off)).w;

   // Sobel filter in X direction
   float sobelX = s00 + 2 * s10 + s20 - s02 - 2 * s12 - s22;
   // Sobel filter in Y direction
   float sobelY = s00 + 2 * s01 + s02 - s20 - 2 * s21 - s22;

   // Find edge, skip sqrt() to improve performance ...
   float edgeSqr = (sobelX * sobelX + sobelY * sobelY);

   // ... and threshold against a squared value instead.
   Out.Color = (edgeSqr > 0.07 * 0.07);
   return Out;
}


//--------------------------------------------------------------//
// Composite
//--------------------------------------------------------------//
string NPR_HatchWithSilhouetteRendering_Composite_ScreenAlignedQuad : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

float4x4 NPR_HatchWithSilhouetteRendering_Composite_Vertex_Shader_view_proj_matrix;
struct NPR_HatchWithSilhouetteRendering_Composite_Vertex_Shader_VS_INPUT
{
   float4 Pos      : POSITION;
};

struct NPR_HatchWithSilhouetteRendering_Composite_Vertex_Shader_VS_OUTPUT
{
   float4 Pos      : POSITION;
   float2 TexCoord : TEXCOORD0;
};

NPR_HatchWithSilhouetteRendering_Composite_Vertex_Shader_VS_OUTPUT NPR_HatchWithSilhouetteRendering_Composite_Vertex_Shader_vs_main( NPR_HatchWithSilhouetteRendering_Composite_Vertex_Shader_VS_INPUT In )
{
   NPR_HatchWithSilhouetteRendering_Composite_Vertex_Shader_VS_OUTPUT Out; 

   In.Pos.xy = sign(In.Pos.xy);
   Out.Pos       = float4(In.Pos.xy, 0.0, 1.0);
   // Image-space
   Out.TexCoord.x = 0.5 * (1 + In.Pos.x);
   Out.TexCoord.y = 0.5 * (1 - In.Pos.y);

   return Out;
}

sampler NPR_HatchWithSilhouetteRendering_Composite_Pixel_Shader_ObjInput = sampler_state
{
   Texture = (RTHatchObj_Tex);
};
sampler NPR_HatchWithSilhouetteRendering_Composite_Pixel_Shader_OutlineInput = sampler_state
{
   Texture = (RTOutlineResult_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
};
struct NPR_HatchWithSilhouetteRendering_Composite_Pixel_Shader_PS_INPUT
{
   float2 TexCoord : TEXCOORD0;
};

struct NPR_HatchWithSilhouetteRendering_Composite_Pixel_Shader_PS_OUTPUT
{
   float4 Color    : COLOR0;
};

NPR_HatchWithSilhouetteRendering_Composite_Pixel_Shader_PS_OUTPUT NPR_HatchWithSilhouetteRendering_Composite_Pixel_Shader_ps_main( NPR_HatchWithSilhouetteRendering_Composite_Pixel_Shader_PS_INPUT In )
{
    NPR_HatchWithSilhouetteRendering_Composite_Pixel_Shader_PS_OUTPUT Out;
 
    float4 objColor     = tex2D(NPR_HatchWithSilhouetteRendering_Composite_Pixel_Shader_ObjInput,In.TexCoord);
    float4 outLineColor = tex2D(NPR_HatchWithSilhouetteRendering_Composite_Pixel_Shader_OutlineInput,In.TexCoord);

    if (outLineColor.x>0.0)
       Out.Color = 0.0;
    else
       Out.Color = objColor;
    
    return Out;
}


//--------------------------------------------------------------//
// MetallicCartoon
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Single Pass
//--------------------------------------------------------------//
string NPR_MetallicCartoon_Single_Pass_Teapot : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\models\\Teapot.3ds";

float4 light1
<
   string UIName = "light1";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 100.00, -100.00, 0.00, 1.00 );
float4 light2
<
   string UIName = "light2";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 0.00, 0.00, 100.00, 1.00 );
float4 view_position : ViewPosition;
float4 light0
<
   string UIName = "light0";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( -100.00, 0.00, 0.00, 1.00 );
float4x4 NPR_MetallicCartoon_Single_Pass_Vertex_Shader_view_proj_matrix : ViewProjection;
struct NPR_MetallicCartoon_Single_Pass_Vertex_Shader_VS_OUTPUT
{
   float4 Pos   : POSITION;
   float3 View  : TEXCOORD0;
   float3 Normal: TEXCOORD1;
   float3 Light1: TEXCOORD2;
   float3 Light2: TEXCOORD3;
   float3 Light3: TEXCOORD4;
};
NPR_MetallicCartoon_Single_Pass_Vertex_Shader_VS_OUTPUT NPR_MetallicCartoon_Single_Pass_Vertex_Shader_main( float4 inPos    : POSITION, 
                float3 inNorm   : NORMAL )
{
   NPR_MetallicCartoon_Single_Pass_Vertex_Shader_VS_OUTPUT Out = (NPR_MetallicCartoon_Single_Pass_Vertex_Shader_VS_OUTPUT) 0;

   // Output transformed vertex position: 
   Out.Pos = mul( NPR_MetallicCartoon_Single_Pass_Vertex_Shader_view_proj_matrix, inPos );

   Out.Normal = inNorm;

   // Compute the view vector: 
   Out.View = normalize( view_position - inPos );

   // Compute vectors to three lights from the current vertex position: 
   Out.Light1 = normalize(light0 - inPos);   // Light 1
   Out.Light2 = normalize(light1 - inPos);   // Light 2
   Out.Light3 = normalize(light2 - inPos);   // Light 3

   return Out;
}






float4 Material
<
   string UIName = "Material";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 0.26, 0.67, 0.27, 1.00 );
texture Outline_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Textures\\NPR Metallic Outline.tga";
>;
sampler Outline = sampler_state
{
   Texture = (Outline_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   ADDRESSW = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
};
float4 NPR_MetallicCartoon_Single_Pass_Pixel_Shader_main( float3 View:   TEXCOORD0,
             float3 Normal: TEXCOORD1,
             float3 Light1: TEXCOORD2,
             float3 Light2: TEXCOORD3,
             float3 Light3: TEXCOORD4 ) : COLOR
{
   // Normalize input normal vector:
   float3 norm = normalize (Normal);
  
   float4 outline = tex1D(Outline, 1 - dot (norm, normalize(View)));

   float lighting = (dot (normalize (Light1), norm) * 0.5 + 0.5) +
                    (dot (normalize (Light2), norm) * 0.5 + 0.5) +
                    (dot (normalize (Light3), norm) * 0.5 + 0.5);

   return outline * lighting * Material; 
}






//--------------------------------------------------------------//
// DilateErode
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// DrawObject
//--------------------------------------------------------------//
string NPR_DilateErode_DrawObject_Elephant : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ElephantBody.3ds";

texture ImageRT_Tex : RenderColorTarget
<
   float2 ViewportRatio={1.0,1.0};
   string Format="D3DFMT_A8R8G8B8";
   float  ClearDepth=1.000000;
   int    ClearColor=0;
>;
float4x4 NPR_DilateErode_DrawObject_Vertex_Shader_view_proj_matrix : ViewProjection;
struct NPR_DilateErode_DrawObject_Vertex_Shader_VS_OUTPUT {
   float4 Pos: POSITION;
};

NPR_DilateErode_DrawObject_Vertex_Shader_VS_OUTPUT NPR_DilateErode_DrawObject_Vertex_Shader_main(float4 Pos: POSITION){
   NPR_DilateErode_DrawObject_Vertex_Shader_VS_OUTPUT Out;

   // Standard mvp transform
   Out.Pos = mul(NPR_DilateErode_DrawObject_Vertex_Shader_view_proj_matrix, Pos);

   return Out;
}



float4 NPR_DilateErode_DrawObject_Pixel_Shader_main() : COLOR {
   // Output white   
   return 1;
}


//--------------------------------------------------------------//
// Dilate
//--------------------------------------------------------------//
string NPR_DilateErode_Dilate_ScreenAlignedQuad : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

float pixelSize
<
   string UIName = "pixelSize";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.01;
> = float( 0.00 );
struct NPR_DilateErode_Dilate_Vertex_Shader_VS_OUTPUT {
   float4 Pos: POSITION;
   float2 texCoord: TEXCOORD;
};

NPR_DilateErode_Dilate_Vertex_Shader_VS_OUTPUT NPR_DilateErode_Dilate_Vertex_Shader_main(float4 Pos: POSITION){
   NPR_DilateErode_Dilate_Vertex_Shader_VS_OUTPUT Out;

   // Clean up inaccuracies
   Pos.xy = sign(Pos.xy);

   Out.Pos = Pos;
   Out.texCoord.x = 0.5 * (1 + Pos.x + pixelSize);
   Out.texCoord.y = 0.5 * (1 - Pos.y + pixelSize);

   return Out;
}




float NPR_DilateErode_Dilate_Pixel_Shader_pixelSize
<
   string UIName = "NPR_DilateErode_Dilate_Pixel_Shader_pixelSize";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.01;
> = float( 0.00 );
sampler ImageRT = sampler_state
{
   Texture = (ImageRT_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
// The surrounding pixels
float2 samples[8] = {
   -1, -1,
    0, -1,
    1, -1,
   -1,  0,
    1,  0,
   -1,  1,
    0,  1,
    1,  1,
};

float4 NPR_DilateErode_Dilate_Pixel_Shader_main(float2 texCoord: TEXCOORD) : COLOR {
   // Simple dilate operation, find the maximum
   // pixel in the neighborhood.
   float4 maxSamp = tex2D(ImageRT, texCoord);
   for (int i = 0; i < 8; i++){
      float4 sample = tex2D(ImageRT, texCoord + NPR_DilateErode_Dilate_Pixel_Shader_pixelSize * samples[i]);
      maxSamp = max(maxSamp, sample);
   }

   return maxSamp;
}







//--------------------------------------------------------------//
// Erode
//--------------------------------------------------------------//
string NPR_DilateErode_Erode_ScreenAlignedQuad : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

float NPR_DilateErode_Erode_Vertex_Shader_pixelSize
<
   string UIName = "NPR_DilateErode_Erode_Vertex_Shader_pixelSize";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.01;
> = float( 0.00 );
struct NPR_DilateErode_Erode_Vertex_Shader_VS_OUTPUT {
   float4 Pos: POSITION;
   float2 texCoord: TEXCOORD;
};

NPR_DilateErode_Erode_Vertex_Shader_VS_OUTPUT NPR_DilateErode_Erode_Vertex_Shader_main(float4 Pos: POSITION){
   NPR_DilateErode_Erode_Vertex_Shader_VS_OUTPUT Out;

   // Clean up inaccuracies
   Pos.xy = sign(Pos.xy);

   Out.Pos = Pos;
   Out.texCoord.x = 0.5 * (1 + Pos.x + NPR_DilateErode_Erode_Vertex_Shader_pixelSize);
   Out.texCoord.y = 0.5 * (1 - Pos.y + NPR_DilateErode_Erode_Vertex_Shader_pixelSize);

   return Out;
}





float NPR_DilateErode_Erode_Pixel_Shader_pixelSize
<
   string UIName = "NPR_DilateErode_Erode_Pixel_Shader_pixelSize";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.01;
> = float( 0.00 );
sampler NPR_DilateErode_Erode_Pixel_Shader_ImageRT = sampler_state
{
   Texture = (ImageRT_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
// We are using reverse subtract blending, so the result will
// be dilate - erode, which is a basic edge-detecting operator.


// The surrounding pixels
float2 NPR_DilateErode_Erode_Pixel_Shader_samples[8] = {
   -1, -1,
    0, -1,
    1, -1,
   -1,  0,
    1,  0,
   -1,  1,
    0,  1,
    1,  1,
};

float4 NPR_DilateErode_Erode_Pixel_Shader_main(float2 texCoord: TEXCOORD) : COLOR {
   // Simple erode operation, find the minimum
   // pixel in the neighborhood.

   float4 minSamp = tex2D(NPR_DilateErode_Erode_Pixel_Shader_ImageRT, texCoord);
   for (int i = 0; i < 8; i++){
      float4 sample = tex2D(NPR_DilateErode_Erode_Pixel_Shader_ImageRT, texCoord + NPR_DilateErode_Erode_Pixel_Shader_pixelSize * NPR_DilateErode_Erode_Pixel_Shader_samples[i]);
      minSamp = min(minSamp, sample);
   }

   return minSamp;
}


//--------------------------------------------------------------//
// Whiteboard
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Draw
//--------------------------------------------------------------//
string NPR_Whiteboard_Draw_Elephant : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ElephantBody.3ds";

texture ImageMap_Tex : RenderColorTarget
<
   float2 RenderTargetDimensions = {512,512};
   string Format="D3DFMT_G16R16";
   float  ClearDepth=1.000000;
   int    ClearColor=0;
>;
float4x4 NPR_Whiteboard_Draw_Vertex_Shader_view_proj_matrix : ViewProjection;
float depthScale
<
   string UIName = "depthScale";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.03;
> = float( 0.00 );
struct NPR_Whiteboard_Draw_Vertex_Shader_VS_OUTPUT {
   float4 Pos: POSITION;
   float texCoord: TEXCOORD;
};

NPR_Whiteboard_Draw_Vertex_Shader_VS_OUTPUT NPR_Whiteboard_Draw_Vertex_Shader_main(float4 Pos: POSITION){
   NPR_Whiteboard_Draw_Vertex_Shader_VS_OUTPUT Out;

   Out.Pos = mul(NPR_Whiteboard_Draw_Vertex_Shader_view_proj_matrix, Pos);
   // Pass depth
   Out.texCoord = depthScale * mul(NPR_Whiteboard_Draw_Vertex_Shader_view_proj_matrix, Pos).z;

   return Out;
}









float4 NPR_Whiteboard_Draw_Pixel_Shader_main(float depth: TEXCOORD) : COLOR {
   return depth;
}



//--------------------------------------------------------------//
// Edge
//--------------------------------------------------------------//
string NPR_Whiteboard_Edge_ScreenAlignedQuad : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

texture EdgeMap_Tex : RenderColorTarget
<
   float2 RenderTargetDimensions = {512,512};
   string Format="D3DFMT_G16R16";
   float  ClearDepth=1.000000;
   int    ClearColor=0;
>;
float4x4 NPR_Whiteboard_Edge_Vertex_Shader_view_proj_matrix;
struct NPR_Whiteboard_Edge_Vertex_Shader_VS_OUTPUT {
   float4 Pos: POSITION;
   float2 texCoord: TEXCOORD;
};

NPR_Whiteboard_Edge_Vertex_Shader_VS_OUTPUT NPR_Whiteboard_Edge_Vertex_Shader_main(float4 Pos: POSITION){
   NPR_Whiteboard_Edge_Vertex_Shader_VS_OUTPUT Out;

   // Clean up inaccuracies
   Pos.xy = sign(Pos.xy);

   Out.Pos = float4(Pos.xy, 0, 1);
   // Image-space
   Out.texCoord.x = 0.5 * (1 + Pos.x);
   Out.texCoord.y = 0.5 * (1 - Pos.y);

   return Out;
}







sampler ImageMap = sampler_state
{
   Texture = (ImageMap_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
// This shader applies a Sobel filter to detect edges in the image.
// The Sobel filter extracts the first order derivates of the image,
// that is, the slope. Where the slope is sharp there is an edge.
// These are the filter kernels:
//
//  SobelX       SobelY
//  1  0 -1      1  2  1
//  2  0 -2      0  0  0
//  1  0 -1     -1 -2 -1


// One pixel offset
const float NPR_Whiteboard_Edge_Pixel_Shader_off = 1.0 / 512.0;

float4 NPR_Whiteboard_Edge_Pixel_Shader_main(float2 texCoord: TEXCOORD) : COLOR {
   // Sample neighbor pixels
   float s00 = tex2D(ImageMap, texCoord + float2(-NPR_Whiteboard_Edge_Pixel_Shader_off, -NPR_Whiteboard_Edge_Pixel_Shader_off)).r;
   float s01 = tex2D(ImageMap, texCoord + float2( 0,   -NPR_Whiteboard_Edge_Pixel_Shader_off)).r;
   float s02 = tex2D(ImageMap, texCoord + float2( NPR_Whiteboard_Edge_Pixel_Shader_off, -NPR_Whiteboard_Edge_Pixel_Shader_off)).r;

   float s10 = tex2D(ImageMap, texCoord + float2(-NPR_Whiteboard_Edge_Pixel_Shader_off,  0)).r;
   float s12 = tex2D(ImageMap, texCoord + float2( NPR_Whiteboard_Edge_Pixel_Shader_off,  0)).r;

   float s20 = tex2D(ImageMap, texCoord + float2(-NPR_Whiteboard_Edge_Pixel_Shader_off,  NPR_Whiteboard_Edge_Pixel_Shader_off)).r;
   float s21 = tex2D(ImageMap, texCoord + float2( 0,    NPR_Whiteboard_Edge_Pixel_Shader_off)).r;
   float s22 = tex2D(ImageMap, texCoord + float2( NPR_Whiteboard_Edge_Pixel_Shader_off,  NPR_Whiteboard_Edge_Pixel_Shader_off)).r;

   // Sobel filter in X direction
   float sobelX = s00 + 2 * s10 + s20 - s02 - 2 * s12 - s22;
   // Sobel filter in Y direction
   float sobelY = s00 + 2 * s01 + s02 - s20 - 2 * s21 - s22;

   // Find edge, skip sqrt() to improve performance ...
   float edgeSqr = (sobelX * sobelX + sobelY * sobelY);
   // ... and threshold against a squared value instead.
   return edgeSqr > 0.07 * 0.07;
}


//--------------------------------------------------------------//
// AntiAlias
//--------------------------------------------------------------//
string NPR_Whiteboard_AntiAlias_ScreenAlignedQuad : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

float4x4 NPR_Whiteboard_AntiAlias_Vertex_Shader_view_proj_matrix;
struct NPR_Whiteboard_AntiAlias_Vertex_Shader_VS_OUTPUT {
   float4 Pos: POSITION;
   float2 texCoord: TEXCOORD;
};

NPR_Whiteboard_AntiAlias_Vertex_Shader_VS_OUTPUT NPR_Whiteboard_AntiAlias_Vertex_Shader_main(float4 Pos: POSITION){
   NPR_Whiteboard_AntiAlias_Vertex_Shader_VS_OUTPUT Out;

   // Clean up inaccuracies
   Pos.xy = sign(Pos.xy);

   Out.Pos = float4(Pos.xy, 0, 1);
   // Image-space
   Out.texCoord.x = 0.5 * (1 + Pos.x);
   Out.texCoord.y = 0.5 * (1 - Pos.y);

   return Out;
}



float hardness
<
   string UIName = "hardness";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 2.00;
> = float( 0.24 );
float AA_SampleDist
<
   string UIName = "AA_SampleDist";
   string UIWidget = "Numeric";
   bool UIVisible =  true;
   float UIMin = 0.00;
   float UIMax = 0.10;
> = float( 0.01 );
sampler EdgeMap = sampler_state
{
   Texture = (EdgeMap_Tex);
};
const float2 NPR_Whiteboard_AntiAlias_Pixel_Shader_samples[12] = {
   -0.326212, -0.405805,
   -0.840144, -0.073580,
   -0.695914,  0.457137,
   -0.203345,  0.620716,
    0.962340, -0.194983,
    0.473434, -0.480026,
    0.519456,  0.767022,
    0.185461, -0.893124,
    0.507431,  0.064425,
    0.896420,  0.412458,
   -0.321940, -0.932615,
   -0.791559, -0.597705,
};

float4 NPR_Whiteboard_AntiAlias_Pixel_Shader_main(float2 texCoord: TEXCOORD) : COLOR {
   // Apply a simple blur filter to get rid of aliasing and
   // get a wider spread of the edges and a softer image.
   float sum = tex2D(EdgeMap, texCoord).r;
   for (int i = 0; i < 12; i++){
      sum += tex2D(EdgeMap, texCoord + AA_SampleDist * NPR_Whiteboard_AntiAlias_Pixel_Shader_samples[i]).r;
   }

   return 1 - hardness * sum;
}



//--------------------------------------------------------------//
// ShowerDoor
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Background
//--------------------------------------------------------------//
string NPR_ShowerDoor_Background_Sphere : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\Sphere.3ds";

texture NPR_ShowerDoor_Background_RT : RenderColorTarget
<
   float2 RenderTargetDimensions = {512,512};
   string Format="D3DFMT_A8R8G8B8";
   float  ClearDepth=1.000000;
   int    ClearColor=-16777216;
>;
float4x4 NPR_ShowerDoor_Background_Vertex_Shader_view_proj_matrix : ViewProjection;
float4 NPR_ShowerDoor_Background_Vertex_Shader_view_position : ViewPosition;

struct NPR_ShowerDoor_Background_Vertex_Shader_VS_INPUT
{
   float4 Pos      : POSITION;
};

struct NPR_ShowerDoor_Background_Vertex_Shader_VS_OUTPUT 
{
   float4 Pos:      POSITION;
   float3 TexCoord: TEXCOORD0;
};

NPR_ShowerDoor_Background_Vertex_Shader_VS_OUTPUT NPR_ShowerDoor_Background_Vertex_Shader_vs_main(NPR_ShowerDoor_Background_Vertex_Shader_VS_INPUT In)
{
   NPR_ShowerDoor_Background_Vertex_Shader_VS_OUTPUT Out;

   // Lock environment to view position
   Out.Pos      = mul(NPR_ShowerDoor_Background_Vertex_Shader_view_proj_matrix, float4(In.Pos.xyz + NPR_ShowerDoor_Background_Vertex_Shader_view_position, 1));
   Out.TexCoord = In.Pos.xyz;

   return Out;
}

texture snow_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Textures\\Snow.dds";
>;
samplerCUBE BGTex = sampler_state
{
   Texture = (snow_Tex);
};

struct NPR_ShowerDoor_Background_Pixel_Shader_PS_INPUT
{
   float3 TexCoord  : TEXCOORD0;
};

struct NPR_ShowerDoor_Background_Pixel_Shader_PS_OUTPUT
{
   float4 Color : COLOR0;
};

NPR_ShowerDoor_Background_Pixel_Shader_PS_OUTPUT NPR_ShowerDoor_Background_Pixel_Shader_ps_main( NPR_ShowerDoor_Background_Pixel_Shader_PS_INPUT In )
{
   //  Output constant color:
   NPR_ShowerDoor_Background_Pixel_Shader_PS_OUTPUT Out;
   Out.Color = texCUBE(BGTex,In.TexCoord);
   return Out;
}




//--------------------------------------------------------------//
// Object
//--------------------------------------------------------------//
string NPR_ShowerDoor_Object_Elephant : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ElephantBody.3ds";

texture NPR_ShowerDoor_Object_RT : RenderColorTarget
<
   float2 RenderTargetDimensions = {512,512};
   string Format="D3DFMT_A8R8G8B8";
   float  ClearDepth=1.000000;
   int    ClearColor=-2697514;
>;
float4x4 matViewProjection : ViewProjection;

struct NPR_ShowerDoor_Object_Vertex_Shader_VS_OUTPUT 
{
   float4 Pos:     POSITION;
};

NPR_ShowerDoor_Object_Vertex_Shader_VS_OUTPUT NPR_ShowerDoor_Object_Vertex_Shader_vs_main( float4 inPos: POSITION )
{
   NPR_ShowerDoor_Object_Vertex_Shader_VS_OUTPUT Out;

   Out.Pos = mul(matViewProjection, inPos);

   return Out;
}




float4 NPR_ShowerDoor_Object_Pixel_Shader_ps_main( float4 inDiffuse: COLOR0 ) : COLOR0
{
   //  Output constant color:
   float4 color;
   color[0] = color[3] = 1.0;
   color[1] = color[2] = 0.0;
   return color;
}




//--------------------------------------------------------------//
// PostProcess
//--------------------------------------------------------------//
string NPR_ShowerDoor_PostProcess_ScreenAlignedQuad : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

float4x4 NPR_ShowerDoor_PostProcess_Vertex_Shader_view_proj_matrix;

struct NPR_ShowerDoor_PostProcess_Vertex_Shader_VS_INPUT
{
   float4 Pos      : POSITION;
   float2 TexCoord : TEXCOORD0;
};

struct NPR_ShowerDoor_PostProcess_Vertex_Shader_VS_OUTPUT
{
   float4 Pos       : POSITION;
   float2 TexCoord  : TEXCOORD0;
};

NPR_ShowerDoor_PostProcess_Vertex_Shader_VS_OUTPUT NPR_ShowerDoor_PostProcess_Vertex_Shader_vs_main( NPR_ShowerDoor_PostProcess_Vertex_Shader_VS_INPUT In )
{
   NPR_ShowerDoor_PostProcess_Vertex_Shader_VS_OUTPUT Out; 

   In.Pos.xy = sign(In.Pos.xy);
   Out.Pos       = float4(In.Pos.xy, 0.0, 1.0);
   
   // Image-space
   Out.TexCoord.x = 0.5 * (1 + In.Pos.x);
   Out.TexCoord.y = 0.5 * (1 - In.Pos.y);

   return Out;
}




samplerCUBE SnowTexture;
sampler2D BaseTexture = sampler_state
{
   Texture = (RT_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
};
texture noise_Tex
<
   string ResourceName = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Textures\\noise.tga";
>;
sampler2D NoiseTexture = sampler_state
{
   Texture = (noise_Tex);
};

struct NPR_ShowerDoor_PostProcess_Pixel_Shader_PS_INPUT
{
   float2 TexCoord  : TEXCOORD0;
};

struct NPR_ShowerDoor_PostProcess_Pixel_Shader_PS_OUTPUT
{
   float4 Color : COLOR0;
};

NPR_ShowerDoor_PostProcess_Pixel_Shader_PS_OUTPUT NPR_ShowerDoor_PostProcess_Pixel_Shader_ps_main( NPR_ShowerDoor_PostProcess_Pixel_Shader_PS_INPUT In )
{
   float2 texCoord = In.TexCoord;
   texCoord += tex2D(NoiseTexture,float2(In.TexCoord)).xy * 0.25;
   
   NPR_ShowerDoor_PostProcess_Pixel_Shader_PS_OUTPUT Out;
   Out.Color = tex2D(BaseTexture,texCoord);
   
   return Out;
}




//--------------------------------------------------------------//
// Silhouette With Normal
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// OutputShape
//--------------------------------------------------------------//
string NPR_Silhouette_With_Normal_OutputShape_Teapot : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\models\\Teapot.3ds";

texture RTColor_Tex : RenderColorTarget
<
   float2 RenderTargetDimensions = {512,512};
   string Format="D3DFMT_A8R8G8B8";
   float  ClearDepth=1.000000;
   int    ClearColor=-16777216;
>;
float4x4 matView : View;
float4x4 NPR_Silhouette_With_Normal_OutputShape_Vertex_Shader_matViewProjection : ViewProjection;

struct NPR_Silhouette_With_Normal_OutputShape_Vertex_Shader_VS_INPUT
{
   float3 Pos:    POSITION;
   float3 Normal: NORMAL;
};

struct NPR_Silhouette_With_Normal_OutputShape_Vertex_Shader_VS_OUTPUT
{
   float4 Pos:    POSITION;
   float3 Normal: TEXCOORD0;
   float  Depth:  TEXCOORD1;
};

NPR_Silhouette_With_Normal_OutputShape_Vertex_Shader_VS_OUTPUT NPR_Silhouette_With_Normal_OutputShape_Vertex_Shader_vs_main( NPR_Silhouette_With_Normal_OutputShape_Vertex_Shader_VS_INPUT In )
{
   NPR_Silhouette_With_Normal_OutputShape_Vertex_Shader_VS_OUTPUT Out = (NPR_Silhouette_With_Normal_OutputShape_Vertex_Shader_VS_OUTPUT) 0; 
   
   Out.Pos     = mul( NPR_Silhouette_With_Normal_OutputShape_Vertex_Shader_matViewProjection, float4(In.Pos,1.0) );
   Out.Normal  = mul( matView, In.Normal );
   Out.Depth   = Out.Pos.z / Out.Pos.w;
   
   return Out;
}

struct NPR_Silhouette_With_Normal_OutputShape_Pixel_Shader_PS_INPUT
{
   float3 Normal: TEXCOORD0;
   float  Depth:  TEXCOORD1;
};

struct NPR_Silhouette_With_Normal_OutputShape_Pixel_Shader_PS_OUTPUT
{
   float4 Color  : COLOR0;
};

NPR_Silhouette_With_Normal_OutputShape_Pixel_Shader_PS_OUTPUT NPR_Silhouette_With_Normal_OutputShape_Pixel_Shader_ps_main( NPR_Silhouette_With_Normal_OutputShape_Pixel_Shader_PS_INPUT In )
{
    NPR_Silhouette_With_Normal_OutputShape_Pixel_Shader_PS_OUTPUT Out;
    
    Out.Color  = float4(1,0,0,0);                            
    
    return Out;
}

//--------------------------------------------------------------//
// OutputNormal
//--------------------------------------------------------------//
string NPR_Silhouette_With_Normal_OutputNormal_Teapot : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\models\\Teapot.3ds";

texture RTNormal_Tex : RenderColorTarget
<
   float2 RenderTargetDimensions = {512,512};
   string Format="D3DFMT_A8R8G8B8";
   float  ClearDepth=1.000000;
   int    ClearColor=0;
>;
float4x4 NPR_Silhouette_With_Normal_OutputNormal_Vertex_Shader_matView : View;
float4x4 NPR_Silhouette_With_Normal_OutputNormal_Vertex_Shader_matViewProjection : ViewProjection;

struct NPR_Silhouette_With_Normal_OutputNormal_Vertex_Shader_VS_INPUT
{
   float3 Pos:    POSITION;
   float3 Normal: NORMAL;
};

struct NPR_Silhouette_With_Normal_OutputNormal_Vertex_Shader_VS_OUTPUT
{
   float4 Pos:    POSITION;
   float3 Normal: TEXCOORD0;
   float  Depth:  TEXCOORD1;
};

NPR_Silhouette_With_Normal_OutputNormal_Vertex_Shader_VS_OUTPUT NPR_Silhouette_With_Normal_OutputNormal_Vertex_Shader_vs_main( NPR_Silhouette_With_Normal_OutputNormal_Vertex_Shader_VS_INPUT In )
{
   NPR_Silhouette_With_Normal_OutputNormal_Vertex_Shader_VS_OUTPUT Out = (NPR_Silhouette_With_Normal_OutputNormal_Vertex_Shader_VS_OUTPUT) 0; 
   
   Out.Pos     = mul( NPR_Silhouette_With_Normal_OutputNormal_Vertex_Shader_matViewProjection, float4(In.Pos,1.0) );
   Out.Normal  = mul( NPR_Silhouette_With_Normal_OutputNormal_Vertex_Shader_matView, In.Normal );
   Out.Depth   = Out.Pos.z / Out.Pos.w;
   
   return Out;
}

struct NPR_Silhouette_With_Normal_OutputNormal_Pixel_Shader_PS_INPUT
{
   float3 Normal: TEXCOORD0;
   float  Depth:  TEXCOORD1;
};

struct NPR_Silhouette_With_Normal_OutputNormal_Pixel_Shader_PS_OUTPUT
{
   float4 Normal : COLOR0;
};

NPR_Silhouette_With_Normal_OutputNormal_Pixel_Shader_PS_OUTPUT NPR_Silhouette_With_Normal_OutputNormal_Pixel_Shader_ps_main( NPR_Silhouette_With_Normal_OutputNormal_Pixel_Shader_PS_INPUT In )
{
    NPR_Silhouette_With_Normal_OutputNormal_Pixel_Shader_PS_OUTPUT Out;
    
    Out.Normal = float4( normalize(In.Normal)*0.5 + 0.5, 0.0 ); 
    
    return Out;
}

//--------------------------------------------------------------//
// CompositeColor
//--------------------------------------------------------------//
string NPR_Silhouette_With_Normal_CompositeColor_ScreenAlignedQuad : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

float4x4 NPR_Silhouette_With_Normal_CompositeColor_Vertex_Shader_view_proj_matrix;
struct NPR_Silhouette_With_Normal_CompositeColor_Vertex_Shader_VS_INPUT
{
   float4 Pos      : POSITION;
   float2 TexCoord : TEXCOORD0;
};

struct NPR_Silhouette_With_Normal_CompositeColor_Vertex_Shader_VS_OUTPUT
{
   float4 Pos      : POSITION;
   float2 TexCoord : TEXCOORD0;
};

NPR_Silhouette_With_Normal_CompositeColor_Vertex_Shader_VS_OUTPUT NPR_Silhouette_With_Normal_CompositeColor_Vertex_Shader_vs_main( NPR_Silhouette_With_Normal_CompositeColor_Vertex_Shader_VS_INPUT In )
{
   NPR_Silhouette_With_Normal_CompositeColor_Vertex_Shader_VS_OUTPUT Out; 

   In.Pos.xy = sign(In.Pos.xy);
   Out.Pos       = float4(In.Pos.xy, 0.0, 1.0);
   // Image-space
   Out.TexCoord.x = 0.5 * (1 + In.Pos.x);
   Out.TexCoord.y = 0.5 * (1 - In.Pos.y);

   return Out;
}

float fInverseViewportWidth;
float fInverseViewportHeight;

sampler NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_RT = sampler_state
{
   Texture = (RTColor_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
};

// One pixel offset
const float NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_off = 1.0 / 512.0;

struct NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_PS_INPUT
{
   float2 TexCoord : TEXCOORD0;
};

struct NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_PS_OUTPUT
{
   float4 Color    : COLOR0;
};


NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_PS_OUTPUT NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_ps_main( NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_PS_INPUT In)
{
   NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_PS_OUTPUT Out;

   // Sample neighbor pixels
   float s00 = tex2D(NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_RT, In.TexCoord + float2(-NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_off,-NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_off)).x;
   float s01 = tex2D(NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_RT, In.TexCoord + float2(   0,-NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_off)).x;
   float s02 = tex2D(NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_RT, In.TexCoord + float2( NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_off,-NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_off)).x;
   
   float s10 = tex2D(NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_RT, In.TexCoord + float2(-NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_off,   0)).x;
   float s12 = tex2D(NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_RT, In.TexCoord + float2( NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_off,   0)).x;
   
   float s20 = tex2D(NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_RT, In.TexCoord + float2(-NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_off, NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_off)).x;
   float s21 = tex2D(NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_RT, In.TexCoord + float2(   0, NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_off)).x;
   float s22 = tex2D(NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_RT, In.TexCoord + float2( NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_off, NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_off)).x;
   
   // Sobel filter in X direction
   float sobelX = s00 + 2 * s10 + s20 - s02 - 2 * s12 - s22;
   // Sobel filter in Y direction
   float sobelY = s00 + 2 * s01 + s02 - s20 - 2 * s21 - s22;

   // Find edge, skip sqrt() to improve performance ...
   float edgeSqr = (sobelX * sobelX + sobelY * sobelY);

   // ... and threshold against a squared value instead.
   Out.Color = 1.0-(edgeSqr > 0.07 * 0.07);
   return Out;
}


//--------------------------------------------------------------//
// CompositeNormal
//--------------------------------------------------------------//
string NPR_Silhouette_With_Normal_CompositeNormal_ScreenAlignedQuad : ModelData = "..\\..\\..\\Program Files (x86)\\AMD\\RenderMonkey 1.81\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

float NPR_Silhouette_With_Normal_CompositeNormal_Vertex_Shader_fInverseViewportWidth;
float NPR_Silhouette_With_Normal_CompositeNormal_Vertex_Shader_fInverseViewportHeight;

float4x4 NPR_Silhouette_With_Normal_CompositeNormal_Vertex_Shader_view_proj_matrix;

struct NPR_Silhouette_With_Normal_CompositeNormal_Vertex_Shader_VS_INPUT
{
   float4 Pos      : POSITION;
   float2 TexCoord : TEXCOORD0;
};

struct NPR_Silhouette_With_Normal_CompositeNormal_Vertex_Shader_VS_OUTPUT
{
   float4 Pos      : POSITION;
   float2 TexCoord : TEXCOORD0;
};

NPR_Silhouette_With_Normal_CompositeNormal_Vertex_Shader_VS_OUTPUT NPR_Silhouette_With_Normal_CompositeNormal_Vertex_Shader_vs_main( NPR_Silhouette_With_Normal_CompositeNormal_Vertex_Shader_VS_INPUT In )
{
   NPR_Silhouette_With_Normal_CompositeNormal_Vertex_Shader_VS_OUTPUT Out; 

   In.Pos.xy = sign(In.Pos.xy);
   Out.Pos       = float4(In.Pos.xy, 0.0, 1.0);
   // Image-space
   Out.TexCoord.x = 0.5 * (1 + In.Pos.x);
   Out.TexCoord.y = 0.5 * (1 - In.Pos.y);

   return Out;
}

sampler NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_RT = sampler_state
{
   Texture = (RTNormal_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
};

// One pixel offset
const float NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_off = 1.0 / 512.0;

const float threadHold = 0.10;

struct NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_PS_INPUT
{
   float2 TexCoord : TEXCOORD0;
};

struct NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_PS_OUTPUT
{
   float4 Color    : COLOR0;
};


NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_PS_OUTPUT NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_ps_main( NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_PS_INPUT In)
{
   NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_PS_OUTPUT Out;

   // Sample center pixel color
   float3 c11 = tex2D(NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_RT, In.TexCoord).xyz * 2.0 - 1.0;
   
   // Sample neighbor pixels
   float  s00 = max(0,dot(c11,tex2D(NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_RT, In.TexCoord + float2(-NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_off,-NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_off)).xyz * 2.0 - 1.0)-threadHold);   
   float  s01 = max(0,dot(c11,tex2D(NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_RT, In.TexCoord + float2(   0,-NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_off)).xyz * 2.0 - 1.0)-threadHold);
   float  s02 = max(0,dot(c11,tex2D(NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_RT, In.TexCoord + float2( NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_off,-NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_off)).xyz * 2.0 - 1.0)-threadHold);
   
   float  s10 = max(0,dot(c11,tex2D(NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_RT, In.TexCoord + float2(-NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_off,   0)).xyz * 2.0 - 1.0)-threadHold);
   float  s12 = max(0,dot(c11,tex2D(NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_RT, In.TexCoord + float2( NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_off,   0)).xyz * 2.0 - 1.0)-threadHold);
   
   float  s20 = max(0,dot(c11,tex2D(NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_RT, In.TexCoord + float2(-NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_off, NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_off)).xyz * 2.0 - 1.0)-threadHold);
   float  s21 = max(0,dot(c11,tex2D(NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_RT, In.TexCoord + float2(   0, NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_off)).xyz * 2.0 - 1.0)-threadHold);
   float  s22 = max(0,dot(c11,tex2D(NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_RT, In.TexCoord + float2( NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_off, NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_off)).xyz * 2.0 - 1.0)-threadHold);
      
   // Sobel filter in X direction
   float sobelX = s00 + 2 * s10 + s20 - s02 - 2 * s12 - s22;
   // Sobel filter in Y direction
   float sobelY = s00 + 2 * s01 + s02 - s20 - 2 * s21 - s22;

   // Find edge, skip sqrt() to improve performance ...
   float edgeSqr = (sobelX * sobelX + sobelY * sobelY);

   // ... and threshold against a squared value instead.
   Out.Color = 1.0-(edgeSqr > 0.07 * 0.07);
   return Out;
}


//--------------------------------------------------------------//
// Technique Section for NPR ASM Effects
//--------------------------------------------------------------//
technique MetallicCartoon_ASM
{
   pass Single_Pass
   {
      VertexShader = (NPR_ASM_Effects_MetallicCartoon_ASM_Single_Pass_Vertex_Shader_);
      PixelShader = (NPR_ASM_Effects_MetallicCartoon_ASM_Single_Pass_Pixel_Shader_);
   }

}

technique Hatch_ASM
{
   pass Single_Pass
   {
      ZENABLE = TRUE;
      FILLMODE = SOLID;
      SHADEMODE = GOURAUD;
      ZWRITEENABLE = TRUE;
      ALPHATESTENABLE = FALSE;
      LASTPIXEL = TRUE;
      SRCBLEND = ONE;
      DESTBLEND = ZERO;
      CULLMODE = NONE;
      ZFUNC = LESS;
      ALPHAREF = 0x0;
      ALPHAFUNC = LESS;
      DITHERENABLE = FALSE;
      ALPHABLENDENABLE = FALSE;
      FOGENABLE = FALSE;
      SPECULARENABLE = FALSE;
      FOGCOLOR = 0xFFFFFFFF;
      FOGTABLEMODE = NONE;
      FOGSTART = 0.000000;
      FOGEND = 0.000000;
      FOGDENSITY = 0.000000;
      STENCILENABLE = FALSE;
      STENCILFAIL = KEEP;
      STENCILZFAIL = KEEP;
      STENCILPASS = KEEP;
      STENCILFUNC = ALWAYS;
      STENCILREF = 0x0;
      STENCILMASK = 0xffffffff;
      STENCILWRITEMASK = 0xffffffff;
      TEXTUREFACTOR = 0x0;
      WRAP0 = 0;
      WRAP1 = 0;
      WRAP2 = 0;
      WRAP3 = 0;
      WRAP4 = 0;
      WRAP5 = 0;
      WRAP6 = 0;
      WRAP7 = 0;
      CLIPPING = FALSE;
      LIGHTING = FALSE;
      AMBIENT = 0x11111111;
      FOGVERTEXMODE = NONE;
      COLORVERTEX = TRUE;
      LOCALVIEWER = TRUE;
      NORMALIZENORMALS = FALSE;
      DIFFUSEMATERIALSOURCE = COLOR1;
      SPECULARMATERIALSOURCE = COLOR2;
      AMBIENTMATERIALSOURCE = COLOR2;
      EMISSIVEMATERIALSOURCE = COLOR2;
      VERTEXBLEND = DISABLE;
      CLIPPLANEENABLE = 0;
      POINTSIZE = 0.000000;
      POINTSIZE_MIN = 0.000000;
      POINTSPRITEENABLE = FALSE;
      POINTSCALEENABLE = FALSE;
      POINTSCALE_A = 0.000000;
      POINTSCALE_B = 0.000000;
      POINTSCALE_C = 0.000000;
      MULTISAMPLEANTIALIAS = TRUE;
      MULTISAMPLEMASK = 0xffffffff;
      PATCHEDGESTYLE = DISCRETE;
      POINTSIZE_MAX = 0.000000;
      INDEXEDVERTEXBLENDENABLE = FALSE;
      COLORWRITEENABLE = RED | GREEN | BLUE | ALPHA;
      TWEENFACTOR = 0.000000;
      BLENDOP = ADD;
      POSITIONDEGREE = CUBIC;
      NORMALDEGREE = LINEAR;

      VertexShader = (NPR_ASM_Effects_Hatch_ASM_Single_Pass_Vertex_Shader_);
      PixelShader = (NPR_ASM_Effects_Hatch_ASM_Single_Pass_Pixel_Shader_);
   }

}

technique GoochLighting_ASM
{
   pass Single_Pass
   {
      ZENABLE = TRUE;
      FILLMODE = SOLID;
      SHADEMODE = GOURAUD;
      ZWRITEENABLE = TRUE;
      ALPHATESTENABLE = FALSE;
      LASTPIXEL = TRUE;
      SRCBLEND = ONE;
      DESTBLEND = ZERO;
      CULLMODE = NONE;
      ZFUNC = LESS;
      ALPHAREF = 0x0;
      ALPHAFUNC = LESS;
      DITHERENABLE = FALSE;
      ALPHABLENDENABLE = FALSE;
      FOGENABLE = FALSE;
      SPECULARENABLE = FALSE;
      FOGCOLOR = 0xFFFFFFFF;
      FOGTABLEMODE = NONE;
      FOGSTART = 0.000000;
      FOGEND = 0.000000;
      FOGDENSITY = 0.000000;
      STENCILENABLE = FALSE;
      STENCILFAIL = KEEP;
      STENCILZFAIL = KEEP;
      STENCILPASS = KEEP;
      STENCILFUNC = ALWAYS;
      STENCILREF = 0x0;
      STENCILMASK = 0xffffffff;
      STENCILWRITEMASK = 0xffffffff;
      TEXTUREFACTOR = 0x0;
      WRAP0 = 0;
      WRAP1 = 0;
      WRAP2 = 0;
      WRAP3 = 0;
      WRAP4 = 0;
      WRAP5 = 0;
      WRAP6 = 0;
      WRAP7 = 0;
      CLIPPING = FALSE;
      LIGHTING = FALSE;
      AMBIENT = 0x11111111;
      FOGVERTEXMODE = NONE;
      COLORVERTEX = TRUE;
      LOCALVIEWER = TRUE;
      NORMALIZENORMALS = FALSE;
      DIFFUSEMATERIALSOURCE = COLOR1;
      SPECULARMATERIALSOURCE = COLOR2;
      AMBIENTMATERIALSOURCE = COLOR2;
      EMISSIVEMATERIALSOURCE = COLOR2;
      VERTEXBLEND = DISABLE;
      CLIPPLANEENABLE = 0;
      POINTSIZE = 0.000000;
      POINTSIZE_MIN = 0.000000;
      POINTSPRITEENABLE = FALSE;
      POINTSCALEENABLE = FALSE;
      POINTSCALE_A = 0.000000;
      POINTSCALE_B = 0.000000;
      POINTSCALE_C = 0.000000;
      MULTISAMPLEANTIALIAS = TRUE;
      MULTISAMPLEMASK = 0xffffffff;
      PATCHEDGESTYLE = DISCRETE;
      POINTSIZE_MAX = 0.000000;
      INDEXEDVERTEXBLENDENABLE = FALSE;
      COLORWRITEENABLE = RED | GREEN | BLUE | ALPHA;
      TWEENFACTOR = 0.000000;
      BLENDOP = ADD;
      POSITIONDEGREE = CUBIC;
      NORMALDEGREE = LINEAR;

      VertexShader = (NPR_ASM_Effects_GoochLighting_ASM_Single_Pass_Vertex_Shader_);
      PixelShader = (NPR_ASM_Effects_GoochLighting_ASM_Single_Pass_Pixel_Shader_);
   }

}

technique ToonWithDynamicSpecular_ASM
{
   pass Single_Pass
   {
      ZENABLE = TRUE;
      FILLMODE = SOLID;
      SHADEMODE = GOURAUD;
      ZWRITEENABLE = TRUE;
      ALPHATESTENABLE = FALSE;
      LASTPIXEL = TRUE;
      SRCBLEND = ONE;
      DESTBLEND = ZERO;
      CULLMODE = NONE;
      ZFUNC = LESS;
      ALPHAREF = 0x0;
      ALPHAFUNC = LESS;
      DITHERENABLE = FALSE;
      ALPHABLENDENABLE = FALSE;
      FOGENABLE = FALSE;
      SPECULARENABLE = FALSE;
      FOGCOLOR = 0xFFFFFFFF;
      FOGTABLEMODE = NONE;
      FOGSTART = 0.000000;
      FOGEND = 0.000000;
      FOGDENSITY = 0.000000;
      STENCILENABLE = FALSE;
      STENCILFAIL = KEEP;
      STENCILZFAIL = KEEP;
      STENCILPASS = KEEP;
      STENCILFUNC = ALWAYS;
      STENCILREF = 0x0;
      STENCILMASK = 0xffffffff;
      STENCILWRITEMASK = 0xffffffff;
      TEXTUREFACTOR = 0x0;
      WRAP0 = 0;
      WRAP1 = 0;
      WRAP2 = 0;
      WRAP3 = 0;
      WRAP4 = 0;
      WRAP5 = 0;
      WRAP6 = 0;
      WRAP7 = 0;
      CLIPPING = FALSE;
      LIGHTING = FALSE;
      AMBIENT = 0x11111111;
      FOGVERTEXMODE = NONE;
      COLORVERTEX = TRUE;
      LOCALVIEWER = TRUE;
      NORMALIZENORMALS = FALSE;
      DIFFUSEMATERIALSOURCE = COLOR1;
      SPECULARMATERIALSOURCE = COLOR2;
      AMBIENTMATERIALSOURCE = COLOR2;
      EMISSIVEMATERIALSOURCE = COLOR2;
      VERTEXBLEND = DISABLE;
      CLIPPLANEENABLE = 0;
      POINTSIZE = 0.000000;
      POINTSIZE_MIN = 0.000000;
      POINTSPRITEENABLE = FALSE;
      POINTSCALEENABLE = FALSE;
      POINTSCALE_A = 0.000000;
      POINTSCALE_B = 0.000000;
      POINTSCALE_C = 0.000000;
      MULTISAMPLEANTIALIAS = TRUE;
      MULTISAMPLEMASK = 0xffffffff;
      PATCHEDGESTYLE = DISCRETE;
      POINTSIZE_MAX = 0.000000;
      INDEXEDVERTEXBLENDENABLE = FALSE;
      COLORWRITEENABLE = RED | GREEN | BLUE | ALPHA;
      TWEENFACTOR = 0.000000;
      BLENDOP = ADD;
      POSITIONDEGREE = CUBIC;
      NORMALDEGREE = LINEAR;

      VertexShader = (NPR_ASM_Effects_ToonWithDynamicSpecular_ASM_Single_Pass_Vertex_Shader_);
      PixelShader = (NPR_ASM_Effects_ToonWithDynamicSpecular_ASM_Single_Pass_Pixel_Shader_);
   }

}

technique ToonWithoutSpecular_ASM
{
   pass Single_Pass
   {
      ZENABLE = TRUE;
      FILLMODE = SOLID;
      SHADEMODE = GOURAUD;
      ZWRITEENABLE = TRUE;
      ALPHATESTENABLE = FALSE;
      LASTPIXEL = TRUE;
      SRCBLEND = ONE;
      DESTBLEND = ZERO;
      CULLMODE = NONE;
      ZFUNC = LESS;
      ALPHAREF = 0x0;
      ALPHAFUNC = LESS;
      DITHERENABLE = FALSE;
      ALPHABLENDENABLE = FALSE;
      FOGENABLE = FALSE;
      SPECULARENABLE = FALSE;
      FOGCOLOR = 0xFFFFFFFF;
      FOGTABLEMODE = NONE;
      FOGSTART = 0.000000;
      FOGEND = 0.000000;
      FOGDENSITY = 0.000000;
      STENCILENABLE = FALSE;
      STENCILFAIL = KEEP;
      STENCILZFAIL = KEEP;
      STENCILPASS = KEEP;
      STENCILFUNC = ALWAYS;
      STENCILREF = 0x0;
      STENCILMASK = 0xffffffff;
      STENCILWRITEMASK = 0xffffffff;
      TEXTUREFACTOR = 0x0;
      WRAP0 = 0;
      WRAP1 = 0;
      WRAP2 = 0;
      WRAP3 = 0;
      WRAP4 = 0;
      WRAP5 = 0;
      WRAP6 = 0;
      WRAP7 = 0;
      CLIPPING = FALSE;
      LIGHTING = FALSE;
      AMBIENT = 0x11111111;
      FOGVERTEXMODE = NONE;
      COLORVERTEX = TRUE;
      LOCALVIEWER = TRUE;
      NORMALIZENORMALS = FALSE;
      DIFFUSEMATERIALSOURCE = COLOR1;
      SPECULARMATERIALSOURCE = COLOR2;
      AMBIENTMATERIALSOURCE = COLOR2;
      EMISSIVEMATERIALSOURCE = COLOR2;
      VERTEXBLEND = DISABLE;
      CLIPPLANEENABLE = 0;
      POINTSIZE = 0.000000;
      POINTSIZE_MIN = 0.000000;
      POINTSPRITEENABLE = FALSE;
      POINTSCALEENABLE = FALSE;
      POINTSCALE_A = 0.000000;
      POINTSCALE_B = 0.000000;
      POINTSCALE_C = 0.000000;
      MULTISAMPLEANTIALIAS = TRUE;
      MULTISAMPLEMASK = 0xffffffff;
      PATCHEDGESTYLE = DISCRETE;
      POINTSIZE_MAX = 0.000000;
      INDEXEDVERTEXBLENDENABLE = FALSE;
      COLORWRITEENABLE = RED | GREEN | BLUE | ALPHA;
      TWEENFACTOR = 0.000000;
      BLENDOP = ADD;
      POSITIONDEGREE = CUBIC;
      NORMALDEGREE = LINEAR;

      VertexShader = (NPR_ASM_Effects_ToonWithoutSpecular_ASM_Single_Pass_Vertex_Shader_);
      PixelShader = (NPR_ASM_Effects_ToonWithoutSpecular_ASM_Single_Pass_Pixel_Shader_);
   }

}

//--------------------------------------------------------------//
// Technique Section for NPR
//--------------------------------------------------------------//
technique Silhouette
{
   pass Pass_0
   <
      string Script = "RenderColorTarget0 = RT_Tex;"
                      "ClearColor = (0, 0, 0, 0);"
                      "ClearDepth = 1.000000;";
   >
   {
      VertexShader = compile vs_1_1 NPR_Silhouette_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_Silhouette_Pass_0_Pixel_Shader_ps_main();
   }

   pass Pass_1
   {
      CULLMODE = NONE;

      VertexShader = compile vs_1_1 NPR_Silhouette_Pass_1_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_Silhouette_Pass_1_Pixel_Shader_ps_main();
   }

}

technique ToonWithDynamicSpecular
{
   pass Single_Pass
   {
      VertexShader = compile vs_1_1 NPR_ToonWithDynamicSpecular_Single_Pass_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_ToonWithDynamicSpecular_Single_Pass_Pixel_Shader_ps_main();
   }

}

technique ToonWithSilhouetteRendering
{
   pass DrawObjToon
   <
      string Script = "RenderColorTarget0 = RTToonObj_Tex;"
                      "ClearColor = (0, 198, 0, 0);"
                      "ClearDepth = 1.000000;";
   >
   {
      VertexShader = compile vs_1_1 NPR_ToonWithSilhouetteRendering_DrawObjToon_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_ToonWithSilhouetteRendering_DrawObjToon_Pixel_Shader_ps_main();
   }

   pass Outline
   <
      string Script = "RenderColorTarget0 = RTOutlineResult_Tex;"
                      "ClearColor = (0, 0, 0, 0);"
                      "ClearDepth = 0.000000;";
   >
   {
      CULLMODE = NONE;

      VertexShader = compile vs_1_1 NPR_ToonWithSilhouetteRendering_Outline_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_ToonWithSilhouetteRendering_Outline_Pixel_Shader_ps_main();
   }

   pass Composite
   {
      VertexShader = compile vs_1_1 NPR_ToonWithSilhouetteRendering_Composite_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_ToonWithSilhouetteRendering_Composite_Pixel_Shader_ps_main();
   }

}

technique Hatch
{
   pass Single_Pass
   {
      VertexShader = compile vs_2_0 NPR_Hatch_Single_Pass_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_Hatch_Single_Pass_Pixel_Shader_ps_main();
   }

}

technique HatchWithSilhouetteRendering
{
   pass DrawObjHatch
   <
      string Script = "RenderColorTarget0 = RTHatchObj_Tex;"
                      "ClearColor = (255, 255, 255, 0);"
                      "ClearDepth = 1.000000;";
   >
   {
      VertexShader = compile vs_2_0 NPR_HatchWithSilhouetteRendering_DrawObjHatch_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_HatchWithSilhouetteRendering_DrawObjHatch_Pixel_Shader_ps_main();
   }

   pass Outline
   <
      string Script = "RenderColorTarget0 = NPR_HatchWithSilhouetteRendering_Outline_RTOutlineResult;"
                      "ClearColor = (0, 0, 0, 0);"
                      "ClearDepth = 0.000000;";
   >
   {
      CULLMODE = NONE;

      VertexShader = compile vs_1_1 NPR_HatchWithSilhouetteRendering_Outline_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_HatchWithSilhouetteRendering_Outline_Pixel_Shader_ps_main();
   }

   pass Composite
   {
      VertexShader = compile vs_1_1 NPR_HatchWithSilhouetteRendering_Composite_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_HatchWithSilhouetteRendering_Composite_Pixel_Shader_ps_main();
   }

}

technique MetallicCartoon
{
   pass Single_Pass
   {
      VertexShader = compile vs_1_1 NPR_MetallicCartoon_Single_Pass_Vertex_Shader_main();
      PixelShader = compile ps_2_0 NPR_MetallicCartoon_Single_Pass_Pixel_Shader_main();
   }

}

technique DilateErode
{
   pass DrawObject
   <
      string Script = "RenderColorTarget0 = ImageRT_Tex;"
                      "ClearColor = (0, 0, 0, 0);"
                      "ClearDepth = 1.000000;";
   >
   {
      ALPHABLENDENABLE = FALSE;

      VertexShader = compile vs_1_1 NPR_DilateErode_DrawObject_Vertex_Shader_main();
      PixelShader = compile ps_2_0 NPR_DilateErode_DrawObject_Pixel_Shader_main();
   }

   pass Dilate
   {
      CULLMODE = NONE;
      ALPHABLENDENABLE = FALSE;
      ZENABLE = FALSE;

      VertexShader = compile vs_1_1 NPR_DilateErode_Dilate_Vertex_Shader_main();
      PixelShader = compile ps_2_0 NPR_DilateErode_Dilate_Pixel_Shader_main();
   }

   pass Erode
   {
      SRCBLEND = ONE;
      DESTBLEND = ONE;
      CULLMODE = NONE;
      ALPHABLENDENABLE = TRUE;
      BLENDOP = REVSUBTRACT;

      VertexShader = compile vs_1_1 NPR_DilateErode_Erode_Vertex_Shader_main();
      PixelShader = compile ps_2_0 NPR_DilateErode_Erode_Pixel_Shader_main();
   }

}

technique Whiteboard
{
   pass Draw
   <
      string Script = "RenderColorTarget0 = ImageMap_Tex;"
                      "ClearColor = (0, 0, 0, 0);"
                      "ClearDepth = 1.000000;";
   >
   {
      CULLMODE = CCW;

      VertexShader = compile vs_1_1 NPR_Whiteboard_Draw_Vertex_Shader_main();
      PixelShader = compile ps_2_0 NPR_Whiteboard_Draw_Pixel_Shader_main();
   }

   pass Edge
   <
      string Script = "RenderColorTarget0 = EdgeMap_Tex;"
                      "ClearColor = (0, 0, 0, 0);"
                      "ClearDepth = 1.000000;";
   >
   {
      CULLMODE = NONE;

      VertexShader = compile vs_1_1 NPR_Whiteboard_Edge_Vertex_Shader_main();
      PixelShader = compile ps_2_0 NPR_Whiteboard_Edge_Pixel_Shader_main();
   }

   pass AntiAlias
   {
      VertexShader = compile vs_1_1 NPR_Whiteboard_AntiAlias_Vertex_Shader_main();
      PixelShader = compile ps_2_0 NPR_Whiteboard_AntiAlias_Pixel_Shader_main();
   }

}

technique ShowerDoor
{
   pass Background
   <
      string Script = "RenderColorTarget0 = NPR_ShowerDoor_Background_RT;"
                      "ClearColor = (0, 0, 0, 255);"
                      "ClearDepth = 1.000000;";
   >
   {
      CULLMODE = NONE;

      VertexShader = compile vs_1_1 NPR_ShowerDoor_Background_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_ShowerDoor_Background_Pixel_Shader_ps_main();
   }

   pass Object
   <
      string Script = "RenderColorTarget0 = NPR_ShowerDoor_Object_RT;"
                      "ClearColor = (214, 214, 214, 255);"
                      "ClearDepth = 1.000000;";
   >
   {
      VertexShader = compile vs_1_1 NPR_ShowerDoor_Object_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_ShowerDoor_Object_Pixel_Shader_ps_main();
   }

   pass PostProcess
   {
      CULLMODE = NONE;

      VertexShader = compile vs_2_0 NPR_ShowerDoor_PostProcess_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_ShowerDoor_PostProcess_Pixel_Shader_ps_main();
   }

}

technique Silhouette_With_Normal
{
   pass OutputShape
   <
      string Script = "RenderColorTarget0 = RTColor_Tex;"
                      "ClearColor = (0, 0, 0, 255);"
                      "ClearDepth = 1.000000;";
   >
   {
      VertexShader = compile vs_1_1 NPR_Silhouette_With_Normal_OutputShape_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_Silhouette_With_Normal_OutputShape_Pixel_Shader_ps_main();
   }

   pass OutputNormal
   <
      string Script = "RenderColorTarget0 = RTNormal_Tex;"
                      "ClearColor = (0, 0, 0, 0);"
                      "ClearDepth = 1.000000;";
   >
   {
      VertexShader = compile vs_1_1 NPR_Silhouette_With_Normal_OutputNormal_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_Silhouette_With_Normal_OutputNormal_Pixel_Shader_ps_main();
   }

   pass CompositeColor
   {
      CULLMODE = NONE;

      VertexShader = compile vs_1_1 NPR_Silhouette_With_Normal_CompositeColor_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_Silhouette_With_Normal_CompositeColor_Pixel_Shader_ps_main();
   }

   pass CompositeNormal
   {
      CULLMODE = NONE;
      ALPHATESTENABLE = TRUE;
      ALPHAREF = 0xf0;
      ALPHAFUNC = LESS;

      VertexShader = compile vs_1_1 NPR_Silhouette_With_Normal_CompositeNormal_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NPR_Silhouette_With_Normal_CompositeNormal_Pixel_Shader_ps_main();
   }

}

