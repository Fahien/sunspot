#include "sunspot/util/Cube.h"

namespace sunspot
{
const char* cube = R"--(
{
    "asset" : {
        "generator" : "Khronos glTF Blender I/O v0.9.11",
        "version" : "2.0"
    },
    "scene" : 0,
    "scenes" : [
        {
            "nodes" : [
                0,
                1,
                2
            ],
            "name" : "Scene"
        }
    ],
    "nodes" : [
        {
            "name" : "Cube",
            "mesh" : 0
        }
    ],
    "materials" : [
        {
            "name" : "Material",
            "pbrMetallicRoughness" : {
                "baseColorTexture" : {
                    "index" : 0,
                    "texCoord" : 0
                }
            }
        }
    ],
    "meshes" : [
        {
            "name" : "Cube",
            "primitives" : [
                {
                    "attributes" : {
                        "NORMAL" : 0,
                        "TEXCOORD_0" : 1,
                        "POSITION" : 2
                    },
                    "indices" : 3,
                    "material" : 0
                }
            ]
        }
    ],
    "textures" : [
        {
            "sampler" : 0,
            "source" : 0
        }
    ],
    "images" : [
        {
            "name" : "Material Diffuse Color",
            "bufferView" : 4,
            "mimeType" : "image/png"
        }
    ],
    "accessors" : [
        {
            "componentType" : 5126,
            "count" : 24,
            "bufferView" : 0,
            "type" : "VEC3"
        },
        {
            "componentType" : 5126,
            "count" : 24,
            "bufferView" : 1,
            "type" : "VEC2"
        },
        {
            "componentType" : 5126,
            "bufferView" : 2,
            "type" : "VEC3",
            "min" : [
                -1.0000003576278687,
                -1,
                -1.0000003576278687
            ],
            "count" : 24,
            "max" : [
                1.0000004768371582,
                1,
                1.0000005960464478
            ]
        },
        {
            "componentType" : 5123,
            "count" : 36,
            "bufferView" : 3,
            "type" : "SCALAR"
        }
    ],
    "bufferViews" : [
        {
            "buffer" : 0,
            "byteOffset" : 0,
            "byteLength" : 288
        },
        {
            "buffer" : 0,
            "byteOffset" : 288,
            "byteLength" : 192
        },
        {
            "buffer" : 0,
            "byteOffset" : 480,
            "byteLength" : 288
        },
        {
            "buffer" : 0,
            "byteOffset" : 768,
            "byteLength" : 72
        },
        {
            "buffer" : 0,
            "byteOffset" : 840,
            "byteLength" : 112
        }
    ],
    "samplers" : [
        {
            "wrapT" : 10497,
            "minFilter" : 9986,
            "wrapS" : 10497,
            "magFilter" : 9729
        }
    ],
    "buffers" : [
        {
            "uri" : "data:application/octet-stream;base64,AAAAAAAAgL8AAACAAAAAAAAAgL8AAACAAAAAAAAAgL8AAACAAAAAAAAAgL8AAACAAAAAAAAAgD8AAAAAAAAAAAAAgD8AAAAAAAAAAAAAgD8AAAAAAAAAAAAAgD8AAAAAAACAP+z/PzMAAJg0AACAP+z/PzMAAJg0AACAP+z/PzMAAJg0AACAP+z/PzMAAJg0AACYtAkA4LMAAIA/AACYtAkA4LMAAIA/AACYtAkA4LMAAIA/AACYtAkA4LMAAIA/AACAv///D7QCAHC0AACAv///D7QCAHC0AACAv///D7QCAHC0AACAv///D7QCAHC0AACANAAAYDQAAIC/AACANAAAYDQAAIC/AACANAAAYDQAAIC/AACANAAAYDQAAIC/wrcqP8S3Kj/n8n8/6fJ/P8K3Kj/p8n8/5/J/P8O3Kj97kKo+5/J/P752UTnCtyo/YJFROenyfz97kKo+wrcqP9rEqj6SnSo/kp0qP9zEqj7YxKo+2sSqPpKdKj+TnSo/28SqPujyfz+SnSo/w7cqP9jEqj7Ctyo/kp0qP+nyfz++dlE5epCqPnqQqj4AgFE5epCqPnqQqj6+dlE5AJBROaZ6UTmTnSo/epCqPtzEqj56kKo+kp0qP752UTnYxKo+AACAPwAAgL///3+/AQCAvwAAgL/9/38/+v9/vwAAgL8DAIC/AACAPwAAgL8AAIA/AwCAvwAAgD/6/38/BACAPwAAgD/3/3+///9/vwAAgD8AAIC/9f9/PwAAgD8FAIA/BACAPwAAgD/3/3+/AACAPwAAgL8AAIA/AACAPwAAgL///3+/9f9/PwAAgD8FAIA/9f9/PwAAgD8FAIA/AQCAvwAAgL/9/38/AACAPwAAgL8AAIA/AwCAvwAAgD/6/38/AwCAvwAAgD/6/38/+v9/vwAAgL8DAIC/AQCAvwAAgL/9/38///9/vwAAgD8AAIC///9/vwAAgD8AAIC/AACAPwAAgL///3+/+v9/vwAAgL8DAIC/BACAPwAAgD/3/3+/AAABAAIAAwABAAAABAAFAAYABAAHAAUACAAJAAoACAALAAkADAANAA4ADAAPAA0AEAARABIAEAATABEAFAAVABYAFAAXABUAiVBORw0KGgoAAAANSUhEUgAAAAQAAAAECAIAAAAmkwkpAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAIklEQVQIHWP8P9ucAQYY/9suh7EZGGdp/4dzWC5cuQbnAADn0Qf6j97CTwAAAABJRU5ErkJggg==",
            "byteLength" : 952
        }
    ]
}
)--";
}
