
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

#include "forms/exception.h"
#include "forms/../exception.h"
#include "kernel/main.h"

/**
 * Dao\Forms\Exception
 *
 * Exceptions thrown in Dao\Forms will use this class
 */
zend_class_entry *dao_forms_exception_ce;

/**
 * Dao\Forms\Exception initializer
 */
DAO_INIT_CLASS(Dao_Forms_Exception){

	DAO_REGISTER_CLASS_EX(Dao\\Forms, Exception, forms_exception, dao_exception_ce, NULL, 0);

	return SUCCESS;
}
