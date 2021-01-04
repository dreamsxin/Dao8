
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

#include "mvc/model/query/statusinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_model_query_statusinterface_ce;

static const zend_function_entry dao_mvc_model_query_statusinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Query_StatusInterface, getModel, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Query_StatusInterface, getMessages, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_Model_Query_StatusInterface, success, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\Model\Query\StatusInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_Model_Query_StatusInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc\\Model\\Query, StatusInterface, mvc_model_query_statusinterface, dao_mvc_model_query_statusinterface_method_entry);

	return SUCCESS;
}

/**
 * Returns the model which executed the action
 *
 * @return Dao\Mvc\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Query_StatusInterface, getModel);

/**
 * Returns the messages produced by a operation failed
 *
 * @return Dao\Mvc\Model\MessageInterface[]
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Query_StatusInterface, getMessages);

/**
 * Allows to check if the executed operation was successful
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_Model_Query_StatusInterface, success);
