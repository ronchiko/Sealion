
#pragma once

#include <stddef.h>

#include "Sealion/matrix.h"

typedef struct __seal_context Seal_Context;

/* Creates an additional window context, and sets it as the current context */
Seal_Context *Seal_NewContext(const char *title, size_t w, size_t h);

/* Discards a context */
void Seal_DestroyContext(Seal_Context *context);

int Seal_SwapContext(Seal_Context *context);

typedef struct {
	Seal_Int2 size;
	Seal_Matrix2x2 matrix; 
} Seal_ContextData;

typedef void (*Seal_RenderPipelineFunction)(Seal_ContextData *, void *);

typedef struct {
	Seal_Context *context;
	Seal_RenderPipelineFunction pipeline;
	void *data;
} Seal_RenderPipeline;

void Seal_BeginPipelines(Seal_RenderPipeline pipelines[], int count);
#define Seal_BeginPipeline(ppln) Seal_BeginPipelines(&(ppln), 1)