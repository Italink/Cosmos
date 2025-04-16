#include "DynamicLambda.h"

void UDynamicLambda::Invoke(void* Params)
{
	if (LambdaInvoker) {
		LambdaInvoker(Params);
	}
}
