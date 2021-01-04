
/*
  +------------------------------------------------------------------------+
  | Dao Framework                                                          |
  +------------------------------------------------------------------------+
  | Copyright (c) 2011-2014 Phalcon Team (http://www.phalconphp.com)       |
  +------------------------------------------------------------------------+
  | This source file is subject to the New BSD License that is bundled     |
  | with this package in the file docs/LICENSE.txt.                        |
  |                                                                        |
  | If you did not receive a copy of the license and are unable to         |
  | obtain it through the world-wide-web, please send an email             |
  | to license@phalconphp.com so we can send you a copy immediately.       |
  +------------------------------------------------------------------------+
  | Authors: Andres Gutierrez <andres@phalconphp.com>                      |
  |          Eduar Carvajal <eduar@phalconphp.com>                         |
  +------------------------------------------------------------------------+
*/

#include "mvc/routerinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_routerinterface_ce;

static const zend_function_entry dao_mvc_routerinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, setDefaultModule, arginfo_dao_mvc_routerinterface_setdefaultmodule)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, setDefaultController, arginfo_dao_mvc_routerinterface_setdefaultcontroller)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, setDefaultAction, arginfo_dao_mvc_routerinterface_setdefaultaction)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, setDefaults, arginfo_dao_mvc_routerinterface_setdefaults)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, handle, arginfo_dao_mvc_routerinterface_handle)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, add, arginfo_dao_mvc_routerinterface_add)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, addGet, arginfo_dao_mvc_routerinterface_addget)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, addPost, arginfo_dao_mvc_routerinterface_addpost)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, addPut, arginfo_dao_mvc_routerinterface_addput)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, addDelete, arginfo_dao_mvc_routerinterface_adddelete)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, addOptions, arginfo_dao_mvc_routerinterface_addoptions)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, addPatch, arginfo_dao_mvc_routerinterface_addpatch)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, addHead, arginfo_dao_mvc_routerinterface_addhead)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, clear, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, getModuleName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, getControllerName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, getActionName, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, getParams, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, getMatchedRoute, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, getMatches, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, wasMatched, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, getRoutes, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, getRouteById, arginfo_dao_mvc_routerinterface_getroutebyid)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, getRouteByName, arginfo_dao_mvc_routerinterface_getroutebyname)
	PHP_ABSTRACT_ME(Dao_Mvc_RouterInterface, isExactControllerName, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\RouterInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_RouterInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc, RouterInterface, mvc_routerinterface, dao_mvc_routerinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets the name of the default module
 *
 * @param string $moduleName
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, setDefaultModule);

/**
 * Sets the default controller name
 *
 * @param string $controllerName
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, setDefaultController);

/**
 * Sets the default action name
 *
 * @param string $actionName
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, setDefaultAction);

/**
 * Sets an array of default paths
 *
 * @param array $defaults
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, setDefaults);

/**
 * Handles routing information received from the rewrite engine
 *
 * @param string $uri
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, handle);

/**
 * Adds a route to the router on any HTTP method
 *
 * @param string $pattern
 * @param string/array $paths
 * @param string $httpMethods
 * @return Dao\Mvc\Router\RouteInterface
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, add);

/**
 * Adds a route to the router that only match if the HTTP method is GET
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\RouteInterface
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, addGet);

/**
 * Adds a route to the router that only match if the HTTP method is POST
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\RouteInterface
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, addPost);

/**
 * Adds a route to the router that only match if the HTTP method is PUT
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\RouteInterface
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, addPut);

/**
 * Adds a route to the router that only match if the HTTP method is DELETE
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\RouteInterface
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, addDelete);

/**
 * Add a route to the router that only match if the HTTP method is OPTIONS
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\RouteInterface
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, addOptions);

/**
 * Add a route to the router that only match if the HTTP method is PATCH
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\RouteInterface
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, addPatch);

/**
 * Adds a route to the router that only match if the HTTP method is HEAD
 *
 * @param string $pattern
 * @param string/array $paths
 * @return Dao\Mvc\Router\RouteInterface
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, addHead);

/**
 * Removes all the defined routes
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, clear);

/**
 * Returns processed module name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, getModuleName);

/**
 * Returns processed controller name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, getControllerName);

/**
 * Returns processed action name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, getActionName);

/**
 * Returns processed extra params
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, getParams);

/**
 * Returns the route that matchs the handled URI
 *
 * @return Dao\Mvc\Router\RouteInterface
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, getMatchedRoute);

/**
 * Return the sub expressions in the regular expression matched
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, getMatches);

/**
 * Check if the router macthes any of the defined routes
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, wasMatched);

/**
 * Return all the routes defined in the router
 *
 * @return Dao\Mvc\Router\RouteInterface[]
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, getRoutes);

/**
 * Returns a route object by its id
 *
 * @param string $id
 * @return Dao\Mvc\Router\RouteInterface
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, getRouteById);

/**
 * Returns a route object by its name
 *
 * @param string $name
 * @return Dao\Mvc\Router\RouteInterface
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, getRouteByName);

/**
 * Returns whether controller name should not be mangled
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, isExactControllerName);

/**
 * Sets the default controller name
 *
 * @param string $controllerName
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, setDefaultController);

/**
 * Returns processed controller name
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_RouterInterface, getControllerName);
