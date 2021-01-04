
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
  |          ZhuZongXin <dreamsxin@qq.com>                                 |
  +------------------------------------------------------------------------+
*/

#include "profiler/exception.h"
#include "profiler/../exception.h"
#include "kernel/main.h"

/**
 * Dao\Profiler\Exception
 *
 * Class for exceptions thrown by Dao\Profiler
 */
zend_class_entry *dao_profiler_exception_ce;

/**
 * Dao\Profiler\Exception initializer
 */
DAO_INIT_CLASS(Dao_Profiler_Exception){

	DAO_REGISTER_CLASS_EX(Dao\\Profiler, Exception, profiler_exception, dao_exception_ce, NULL, 0);

	return SUCCESS;
}
