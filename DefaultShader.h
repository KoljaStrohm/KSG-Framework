#pragma once

#include "Text.h"
namespace Framework
{
    // Gibt einen simplen Vertex Shader zurück
    //  ret: Eine Reference auf ein Text objekt, in dem der Shader gespeichert werden soll
    void getVertexShader( Text &ret )
    {
        ret = "////////////////////////////////////////////////////////////////////////////////         \n\
        /////////////                                                                                   \n\
        // GLOBALS //                                                                                   \n\
        /////////////                                                                                   \n\
        cbuffer MatrixBuffer : register( b0 )                                                           \n\
        {                                                                                               \n\
            matrix knochenMatrix[ "; ret += MAX_KNOCHEN_ANZ; ret += " ];                                                                \n\
        };                                                                                              \n\
                                                                                                        \n\
        //////////////                                                                                  \n\
        // TYPEDEFS //                                                                                  \n\
        //////////////                                                                                  \n\
        struct VertexInputType                                                                          \n\
        {                                                                                               \n\
            float4 position : POSITION;                                                                 \n\
            float2 tex : TEXCOORD0;                                                                     \n\
            uint knochen : KNOCHEN_ID0;                                                                 \n\
        };                                                                                              \n\
                                                                                                        \n\
        struct PixelInputType                                                                           \n\
        {                                                                                               \n\
            float4 position : SV_POSITION;                                                              \n\
            float2 tex : TEXCOORD0;                                                                     \n\
        };                                                                                              \n\
                                                                                                        \n\
        ////////////////////////////////////////////////////////////////////////////////                \n\
        // Vertex Shader                                                                                \n\
        ////////////////////////////////////////////////////////////////////////////////                \n\
        PixelInputType TextureVertexShader( VertexInputType input )                                     \n\
        {                                                                                               \n\
            //return input;                                                                             \n\
            PixelInputType output;                                                                      \n\
                                                                                                        \n\
            // Change the position vector to be 4 units for proper matrix calculations.                 \n\
            input.position.w = 1.0f;                                                                    \n\
                                                                                                        \n\
            // Store the texture coordinates for the pixel shader.                                      \n\
            output.tex = input.tex;                                                                     \n\
                                                                                                        \n\
            // Calculate the position of the vertex against the world, view, and projection matrices.   \n\
            output.position = mul( input.position, knochenMatrix[ input.knochen ] );                    \n\
                                                                                                        \n\
            return output;                                                                              \n\
        }";
    }


    // Gibt einen simplen Pixel Shader zurück
    //  ret: Eine Reference auf ein Text objekt, in dem der Shader gespeichert werden soll
    void getPixelShader( Text &ret )
    {
        ret = "////////////////////////////////////////////////////////////////////////////////                  \n\
        // Filename: texture.ps                                                                                  \n\
        ////////////////////////////////////////////////////////////////////////////////                         \n\
                                                                                                                 \n\
                                                                                                                 \n\
        /////////////                                                                                            \n\
        // GLOBALS //                                                                                            \n\
        /////////////                                                                                            \n\
        Texture2D shaderTexture;                                                                                 \n\
        SamplerState SampleType;                                                                                 \n\
                                                                                                                 \n\
                                                                                                                 \n\
        //////////////                                                                                           \n\
        // TYPEDEFS //                                                                                           \n\
        //////////////                                                                                           \n\
        struct PixelInputType                                                                                    \n\
        {                                                                                                        \n\
            float4 position : SV_POSITION;                                                                       \n\
            float2 tex : TEXCOORD0;                                                                              \n\
        };                                                                                                       \n\
                                                                                                                 \n\
                                                                                                                 \n\
        ////////////////////////////////////////////////////////////////////////////////                         \n\
        // Pixel Shader                                                                                          \n\
        ////////////////////////////////////////////////////////////////////////////////                         \n\
        float4 TexturePixelShader( PixelInputType input ) : SV_TARGET                                            \n\
        {                                                                                                        \n\
            //return float4( 0.5, 0.5, 0.5, 0.5 );                                                               \n\
            // Sample the pixel color from the texture using the sampler at this texture coordinate location.    \n\
            float4 textureColor = shaderTexture.Sample( SampleType, input.tex );                                 \n\
            return textureColor;                                                                                 \n\
        }";
    }
}