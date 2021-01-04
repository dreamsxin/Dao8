
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

#include "session/baginterface.h"
#include "kernel/main.h"

zend_class_entry *dao_session_baginterface_ce;

static const zend_function_entry dao_session_baginterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Session_BagInterface, initialize, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Session_BagInterface, destroy, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Session_BagInterface, set, arginfo_dao_session_baginterface_set)
	PHP_ABSTRACT_ME(Dao_Session_BagInterface, get, arginfo_dao_session_baginterface_get)
	PHP_ABSTRACT_ME(Dao_Session_BagInterface, has, arginfo_dao_session_baginterface_has)
	PHP_ABSTRACT_ME(Dao_Session_BagInterface, remove, arginfo_dao_session_baginterface_remove)
	PHP_FE_END
};

/**
 * Dao\Session\BagInterface initializer
 */
DAO_INIT_CLASS(Dao_Session_BagInterface){

	DAO_REGISTER_INTERFACE(Dao\\Session, BagInterface, session_baginterface, dao_session_baginterface_method_entry);

	return SUCCESS;
}

/**
 * Initializes the session bag. This method must not be called directly, the class calls it when its internal data is accesed
 */
DAO_DOC_METHOD(Dao_Session_BagInterface, initialize);

/**
 * Destroyes the session bag
 */
DAO_DOC_METHOD(Dao_Session_BagInterface, destroy);

/**
 * Setter of values
 *
 * @param string $property
 * @param string $value
 */
DAO_DOC_METHOD(Dao_Session_BagInterface, set);

/**
 * Getter of values
 *
 * @param string $property
 * @param mixed $defaultValue
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Session_BagInterface, get);

/**
 * Isset property
 *
 * @param string $property
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Session_BagInterface, has);

/**
 * Unset property
 *
 * @param string $property
 */
DAO_DOC_METHOD(Dao_Session_BagInterface, remove);
