
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

#include "mvc/urlinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_urlinterface_ce;

static const zend_function_entry dao_mvc_urlinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_UrlInterface, setBaseUri, arginfo_dao_mvc_urlinterface_setbaseuri)
	PHP_ABSTRACT_ME(Dao_Mvc_UrlInterface, getBaseUri, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_UrlInterface, setBasePath, arginfo_dao_mvc_urlinterface_setbasepath)
	PHP_ABSTRACT_ME(Dao_Mvc_UrlInterface, getBasePath, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_UrlInterface, get, arginfo_dao_mvc_urlinterface_get)
	PHP_ABSTRACT_ME(Dao_Mvc_UrlInterface, path, arginfo_dao_mvc_urlinterface_path)
	PHP_FE_END
};

/**
 * Dao\Mvc\UrlInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_UrlInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc, UrlInterface, mvc_urlinterface, dao_mvc_urlinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets a prefix to all the urls generated
 *
 * @param string $baseUri
 */
DAO_DOC_METHOD(Dao_Mvc_UrlInterface, setBaseUri);

/**
 * Returns the prefix for all the generated urls. By default /
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_UrlInterface, getBaseUri);

/**
 * Sets a base paths for all the generated paths
 *
 * @param string $basePath
 */
DAO_DOC_METHOD(Dao_Mvc_UrlInterface, setBasePath);

/**
 * Returns a base path
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_UrlInterface, getBasePath);

/**
 * Generates a URL
 *
 * @param string|array $uri
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_UrlInterface, get);

/**
 * Generates a local path
 *
 * @param string $path
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_UrlInterface, path);
