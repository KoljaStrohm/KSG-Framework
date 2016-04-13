/////////////                                                                                  
// GLOBALS //                                                                                  
/////////////                                                                                  
cbuffer MatrixBuffer : register( b0 )
{
    matrix knochenMatrix[ 128 ];
};

//////////////                                                                                 
// TYPEDEFS //                                                                                 
//////////////                                                                                 
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    int knochen : KNOCHEN_ID;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

////////////////////////////////////////////////////////////////////////////////               
// Vertex Shader                                                                               
////////////////////////////////////////////////////////////////////////////////               
PixelInputType TextureVertexShader( VertexInputType input )
{
    //return input;                                                                            
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.                
    input.position.w = 1.0f;

    // Store the texture coordinates for the pixel shader.                                     
    output.tex = input.tex;

    // Calculate the position of the vertex against the world, view, and projection matrices.  
    output.position = mul( input.position, knochenMatrix[ input.knochen ] );

    return output;
}