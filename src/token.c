#include <coulang/token.h>

void
deinit__CoulangToken(const CoulangToken *const self)
{
	switch (self->kind) {
		case COULANG_TOKEN_KIND_IDENTIFIER:
			deinit__String(&self->identifier);

			break;
		case COULANG_TOKEN_KIND_STRING:
			deinit__String(&self->string);

			break;
		case COULANG_TOKEN_KIND_INTEGER:
			deinit__String(&self->integer);

			break;
		case COULANG_TOKEN_KIND_FLOAT:
			deinit__String(&self->float_);

			break;
		default:
			break;
	}
}
