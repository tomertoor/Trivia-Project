#include "IRequestHandler.h"

class MenuRequestHandler : public IRequestHandler
{
public:
    MenuRequestHandler();
	~MenuRequestHandler();

	bool isRequestRelevant(Requests::RequestInfo request) override;
	Requests::RequestResult handleRequest(Requests::RequestInfo request) override;
};