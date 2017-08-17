
#include "graphics\Context.h"

int main()
{
	Context context;

	context.init(800, 600, "GRAPHICS!!!!");

	while (context.step())
	{

	}

	context.term();

	return 0;
}