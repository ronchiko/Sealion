
# Sealion Engine Standards & Requirements

## Shaders

If its intented to use a `Seal_Batcher` with a custom shader please follow these guideline on your shader

### Attributes 
Each program must have the following attributes (with the same names)
```glsl
in vec2 _Seal_Vertex;
in vec4 _Seal_Tint;
in mat3 _Seal_Transfrom;
```
Otherwise, `Seal_NewBatcher` will fail.
If more attributes are required, the must be bound on explicitly by the program. 

### Uniforms
No uniforms are currently required to be used by the built in rendering.

### Passing values to shaders
If a `Seal_Matrix<N>x<N>` should be passed to a shader (as uniform or attribute) please make
sure to call `Seal_M<N>Transpose` on the matrix.

If there is a need to use a matrix as an attribute please copy this line in the top of the file
```c
extern void void Seal_GL_BindMatrix(Seal_GL_Buffer vbo, int position, size_t offset, int n)
```
And use this function to bind the matrix.
