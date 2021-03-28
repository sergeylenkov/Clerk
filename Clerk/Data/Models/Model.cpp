#include "Model.h"

using namespace Clerk::Data;

bool Model::operator== (const Clerk::Data::Model& model)
{
    return model.id == this->id;
}