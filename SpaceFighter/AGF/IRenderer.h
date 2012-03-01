#pragma once

class IRenderer
{
public:
	IRenderer(void);
	virtual ~IRenderer(void);

	BOOL Initialize();
	BOOL RenderBegin();
	BOOL RenderEnd();
	BOOL Present();
	BOOL ShutDown();

protected:

private:
};
