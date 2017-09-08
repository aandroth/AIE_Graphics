
#include "graphics\Context.h"
#include "graphics\Vertex.h"
#include "graphics\draw.h"
#include "graphics\load.h"
#include <vector>
using std::vector;
#include "graphics\glm\ext.hpp"

int main()
{
	Context context;

	int loc, slot;
	while (context.step())
	{
		for (int ii = 0; ii < 2; ++ii)
		{
			loc = slot = 0;
			setUniforms(gpass, loc, slot, cam, objects[ii]);
			s0_draw(gbuffer, gpass, objects[ii].geo);
		}

		/////////////////////
		// LPass
		clearFramebuffer(lbuffer);
		for (int ii = 0; ii < 2; ++ii)
		{
			////////////////////////
			// SPass
			clearFramebuffer(sbuffer);
			setFlags(RenderFlag::DEPTH);
			for (int jj = 0; jj < 3; ++jj)
			{
				loc = slot = 0;
				setUniforms(spassD, loc, slot, dlights[ii].getProj(), dlights[ii].getView(), objects[jj].mo)
				s0_draw(sbuffer, spassD, objects[jj].geo);
			}

			// LPass
			setFlags(RenderFlag::ADDITIVE);
			loc = slot = 0;
			setUniforms(lpassD, loc, slot, cam, dlights[ii], gbuffer.targets[3], gbuffer.targets[2], sbuffer.depth);
			s0_draw(lbuffer, lpassD, quad);
		}
	}


}