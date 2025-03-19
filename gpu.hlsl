
cbuffer constants : register(b0)
{
    float2 rn_screensize;
    float2 r_texturesize;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

struct spritedesc
{
    float2 location;
    float2 size;
    float2 anchor;
    float2 position;
    float2 scale;
    float  rotation;
    uint   color;
};

struct pixeldesc
{
    float4 position : SV_POSITION;
    float2 location : LOC;
    float4 color    : COL;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

StructuredBuffer<spritedesc> spritebatch : register(t0);
Texture2D                    spritesheet : register(t1);

SamplerState                 aaptsampler : register(s0);

///////////////////////////////////////////////////////////////////////////////////////////////////

pixeldesc vs(uint spriteid : SV_INSTANCEID, uint vertexid : SV_VERTEXID)
{
    spritedesc sprite = spritebatch[spriteid];

    uint2  idx = { vertexid & 2, (vertexid << 1 & 2) ^ 3 };

    float4 piv = float4(0, 0, sprite.size + 1) * sprite.scale.xyxy - (sprite.size * sprite.scale * sprite.anchor).xyxy;
    float2 pos = float2(piv[idx.x] * cos(sprite.rotation) - piv[idx.y] * sin(sprite.rotation), piv[idx.y] * cos(sprite.rotation) + piv[idx.x] * sin(sprite.rotation)) + sprite.position - 0.5f;

    float4 loc = float4(sprite.location, sprite.location + sprite.size + 1);

    pixeldesc output;
   
    output.position = float4(pos * rn_screensize - float2(1, -1), 0, 1);
    output.location = float2(loc[idx.x], loc[idx.y]);
    output.color    = float4(sprite.color >> uint4(16, 8, 0, 24) & 0xff) / 255;

    return output;
}

float4 ps(pixeldesc pixel) : SV_TARGET
{
    float4 color = spritesheet.Sample(aaptsampler, (floor(pixel.location) + min(frac(pixel.location) / fwidth(pixel.location), 1.0f) - 0.5f) * r_texturesize) * pixel.color.a * float4(pixel.color.rgb, 1);
    
    if (color.a == 0) discard;

    return color;
}
