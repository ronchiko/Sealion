
# Sealion Engine Standards & Requirements

## Shaders

If its intented to use a <code>Seal_Batcher</code> with a custom shader please follow these guideline on your shader

### Attributes 
Each program must have the following attributes (with the same names)
<code> 
	in vec2 _Seal_Vertex;
	in vec4 _Seal_Tint;
	in mat3 _Seal_Transfrom;
</code>
Otherwise, <code>Seal_NewBatcher</code> will fail.
If more attributes are required, the must be bound on explicitly by the program. 

### Uniforms
No uniforms are currently required to be used by the built in rendering.

### Passing values to shaders
If a <code>Seal_MatrixNxN</code> should be passed to a shader (as uniform or attribute) please make
sure to call <code>Seal_MNTranspose</code> on the matrix.

If there is a need to use a matrix as an attribute please copy this line in the top of the file
<code>extern void void Seal_GL_BindMatrix(Seal_GL_Buffer vbo, int position, size_t offset, int n)</code>
And use this function to bind the matrix.