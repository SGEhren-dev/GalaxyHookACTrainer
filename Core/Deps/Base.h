#pragma once

#define BIT(x) (1 << x)

#define safe_delete(x) if(x) delete x;

#define safe_release(x) if(x) x->Release();
