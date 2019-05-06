#version 330

#define M_PI 3.141592653

layout (points) in;
layout (triangle_strip, max_vertices = 256) out;


uniform mat4 transform;
uniform int numControlPoints;
uniform int numSides;
uniform float rootRadius;
uniform float tipRadius;
uniform float curvature;


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

    vec4 root = gl_in[0].gl_Position;
    
    if(numSides > 0 && numControlPoints > 0)
    {
        float ratio = 1.f/numControlPoints;

        vec4 tangent = vec4(1, 0, 0, 0);
        vec4 direction = vec4(0, 1, 0, 0);
        vec4 binormal = vec4(0, 0, 1, 0);

        mat4 segmentTransform;

        segmentTransform[0] = vec4(cos(curvature), -sin(curvature), 0, 0); // first column.
        segmentTransform[1] = vec4(sin(curvature), cos(curvature), 0, 0); // second column.
        segmentTransform[2] = vec4(0, 0, 1, 0); // third column.
        segmentTransform[3] = vec4(0, 0, 0, 1); // fourth column. Translation.
        
        vec4 previousPoint = root;

        for(int cp=0; cp<numControlPoints; ++cp)
        {
            vec4 newTangent = segmentTransform * tangent;
            vec4 newDirection = segmentTransform * direction;
            vec4 newBinormal = segmentTransform * binormal;
            
            vec4 newPoint = previousPoint + newDirection * ratio*ex_Length[0];

            float segmentBeginRadius = rootRadius + (tipRadius - rootRadius) * cp * ratio;
            float segmentEndRadius = rootRadius + (tipRadius - rootRadius) * (cp+1) * ratio;
            for(int i=0; i<numSides; ++i)
            {
                float val = (2*M_PI*i)/numSides;

                vec4 vertBegin = previousPoint + tangent * segmentBeginRadius * cos(val) + binormal * segmentBeginRadius * sin(val);
                vec4 vertEnd = newPoint + newTangent * segmentEndRadius * cos(val) + newBinormal * segmentEndRadius * sin(val);


                gl_Position = transform * vertBegin;
                EmitVertex();

                gl_Position = transform * vertEnd;
                EmitVertex();
            }

            vec4 vertBegin = previousPoint + tangent * segmentBeginRadius;
            vec4 vertEnd = newPoint + newTangent * segmentEndRadius;

            gl_Position = transform * vertBegin;
            EmitVertex();

            gl_Position = transform * vertEnd;
            EmitVertex();

            EndPrimitive();

            previousPoint = newPoint;
            
            tangent = newTangent;
            direction = newDirection;
            binormal = newBinormal;
        }
    }
}