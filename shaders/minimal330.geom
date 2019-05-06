#version 330

#define M_PI 3.141592653

layout (points) in;
layout (triangle_strip, max_vertices = 256) out;


uniform mat4 transform;
uniform int numControlPoints;
uniform int numSides;
uniform float rootRadius;
uniform float tipRadius;


// In
in vec3 ex_Color[];
in float ex_Length[];
// varying in vec3 ex_Direction[];

// Out
out vec3 frag_ex_Color;

// --------------------
void main()
{
    frag_ex_Color = ex_Color[0];

    vec4 p = gl_in[0].gl_Position;
    
    if(numSides > 0 && numControlPoints > 0)
    {
        float ratio = 1.f/numControlPoints;
        for(int cp=0; cp<numControlPoints; ++cp)
        {
            float segmentBegin = cp * ratio * ex_Length[0];
            float segmentEnd = (cp+1) * ratio * ex_Length[0];

            float segmentBeginRadius = rootRadius + (tipRadius - rootRadius) * cp * ratio;
            float segmentEndRadius = rootRadius + (tipRadius - rootRadius) * (cp+1) * ratio;
            for(int i=0; i<numSides; ++i)
            {
                float val = (2*M_PI*i)/numSides;

                float xRoot = segmentBeginRadius * cos(val);
                float zRoot = segmentBeginRadius * sin(val);

                float xTip = segmentEndRadius * cos(val);
                float zTip = segmentEndRadius * sin(val);

                gl_Position = transform * (p + vec4(xRoot, segmentBegin, zRoot, 0));
                EmitVertex();

                gl_Position = transform * (p + vec4(xTip, segmentEnd, zTip, 0));
                EmitVertex();
            }

            gl_Position = transform * (p + vec4(segmentBeginRadius, segmentBegin, 0, 0));
            EmitVertex();

            gl_Position = transform * (p + vec4(segmentEndRadius, segmentEnd, 0, 0));
            EmitVertex();

            EndPrimitive();
        }
    }
}