#include "root.h"

#include <Cutelyst/Plugins/Authentication/authentication.h>

using namespace Cutelyst;

Root::Root(QObject *parent) : Controller(parent) {}

Root::~Root() {}

void Root::index(Context *c) { c->response()->body() = "Welcome to Cutelyst!"; }

void Root::defaultPage(Context *c) {
  c->response()->body() = "Page not found!";
  c->response()->setStatus(404);
}

bool Root::Auto(Context *c) {
  // User found, so return true to continue with processing after this 'auto'
  return true;
}
