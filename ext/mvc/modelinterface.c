
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

#include "mvc/modelinterface.h"
#include "kernel/main.h"

zend_class_entry *dao_mvc_modelinterface_ce;

static const zend_function_entry dao_mvc_modelinterface_method_entry[] = {
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, setTransaction, arginfo_dao_mvc_modelinterface_settransaction)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getSource, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getSchema, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getIdentityField, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getColumnMap, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getReverseColumnMap, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getAttributes, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getPrimaryKeyAttributes, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getNonPrimaryKeyAttributes, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getNotNullAttributes, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getDataTypesNumeric, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, isNotNull, arginfo_dao_mvc_modelinterface_isnotnull)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getDataTypes, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getDataSize, arginfo_dao_mvc_modelinterface_getdatasize)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getDataByte, arginfo_dao_mvc_modelinterface_getdatabyte)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getDataScale, arginfo_dao_mvc_modelinterface_getdatascale)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getBindTypes, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getDefaultValues, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getAutomaticCreateAttributes, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getAutomaticUpdateAttributes, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, hasRealAttribute, arginfo_dao_mvc_modelinterface_hasrealattribute)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getRealAttribute, arginfo_dao_mvc_modelinterface_getrealattribute)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, hasAttribute, arginfo_dao_mvc_modelinterface_hasattribute)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getAttribute, arginfo_dao_mvc_modelinterface_getattribute)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, setConnectionService, arginfo_dao_mvc_modelinterface_setconnectionservice)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, setWriteConnectionService, arginfo_dao_mvc_modelinterface_setwriteconnectionservice)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, setReadConnectionService, arginfo_dao_mvc_modelinterface_setreadconnectionservice)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getReadConnectionService, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getWriteConnectionService, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getReadConnection, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getWriteConnection, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, assign, arginfo_dao_mvc_modelinterface_assign)
	ZEND_FENTRY(cloneResultMap, NULL, arginfo_dao_mvc_modelinterface_cloneresultmap, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	ZEND_FENTRY(cloneResult, NULL, arginfo_dao_mvc_modelinterface_cloneresult, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	ZEND_FENTRY(cloneResultMapHydrate, NULL, arginfo_dao_mvc_modelinterface_cloneresultmaphydrate, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	ZEND_FENTRY(find, NULL, arginfo_dao_mvc_modelinterface_find, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	ZEND_FENTRY(findFirst, NULL, arginfo_dao_mvc_modelinterface_findfirst, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	ZEND_FENTRY(query, NULL, arginfo_dao_mvc_modelinterface_query, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	ZEND_FENTRY(count, NULL, arginfo_dao_mvc_modelinterface_count, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	ZEND_FENTRY(sum, NULL, arginfo_dao_mvc_modelinterface_sum, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	ZEND_FENTRY(maximum, NULL, arginfo_dao_mvc_modelinterface_maximum, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	ZEND_FENTRY(minimum, NULL, arginfo_dao_mvc_modelinterface_minimum, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	ZEND_FENTRY(average, NULL, arginfo_dao_mvc_modelinterface_average, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, appendMessage, arginfo_dao_mvc_modelinterface_appendmessage)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, validationHasFailed, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getMessages, arginfo_dao_mvc_modelinterface_getmessages)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, save, arginfo_dao_mvc_modelinterface_save)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, create, arginfo_dao_mvc_modelinterface_create)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, update, arginfo_dao_mvc_modelinterface_update)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, delete, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getOperationMade, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, refresh, arginfo_empty)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, readAttribute, arginfo_dao_mvc_modelinterface_readattribute)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, writeAttribute, arginfo_dao_mvc_modelinterface_writeattribute)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, getRelated, arginfo_dao_mvc_modelinterface_getrelated)
	ZEND_FENTRY(remove, NULL, arginfo_dao_mvc_modelinterface_remove, ZEND_ACC_STATIC|ZEND_ACC_ABSTRACT|ZEND_ACC_PUBLIC)
	PHP_ABSTRACT_ME(Dao_Mvc_ModelInterface, reset, arginfo_empty)
	PHP_FE_END
};

/**
 * Dao\Mvc\ModelInterface initializer
 */
DAO_INIT_CLASS(Dao_Mvc_ModelInterface){

	DAO_REGISTER_INTERFACE(Dao\\Mvc, ModelInterface, mvc_modelinterface, dao_mvc_modelinterface_method_entry);

	return SUCCESS;
}

/**
 * Sets a transaction related to the Model instance
 *
 * @param Dao\Mvc\Model\TransactionInterface $transaction
 * @return Dao\Mvc\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, setTransaction);

/**
 * Returns table name mapped in the model
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getSource);

/**
 * Returns schema name where table mapped is located
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getSchema);

/**
 * Returns the name of identity field (if one is present)
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getIdentityField)

/**
 * Returns the column map if any
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getColumnMap)

/**
 * Returns the reverse column map if any
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getReverseColumnMap)

/**
 * Returns table attributes names (fields)
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getAttributes)

/**
 * Returns an array of fields which are part of the primary key
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getPrimaryKeyAttributes)

/**
 * Returns an arrau of fields which are not part of the primary key
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getNonPrimaryKeyAttributes)

/**
 * Returns an array of not null attributes
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getNotNullAttributes)

/**
 * Returns attributes which types are numerical
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getDataTypesNumeric)

/**
 * Checks if the attribute is not null
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, isNotNull)

/**
 * Returns the columns data types
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getDataTypes)

/**
 * Returns attribute data size
 *
 * @param string $attribute
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getDataSize)

/**
 * Returns attribute data byte
 *
 * @param string $attribute
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getDataByte)

/**
 * Returns attribute data scale
 *
 * @param string $attribute
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getDataScale)

/**
 * Returns attributes and their bind data types
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getBindTypes)

/**
 * Returns attributes and their default values
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getDefaultValues)

/**
 * Returns attributes that must be ignored from the INSERT SQL generation
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getAutomaticCreateAttributes)

/**
 * Returns attributes that must be ignored from the UPDATE SQL generation
 *
 * @return array
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getAutomaticUpdateAttributes)

/**
 * Check if a model has certain column
 *
 * @param string $column
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, hasRealAttribute)

/**
 * Gets a model certain column
 *
 * @param string $column
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getColumn)

/**
 * Check if a model has certain attribute
 *
 * @param string $attribute
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, hasAttribute)

/**
 * Gets a model certain attribute
 *
 * @param string $attribute
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getAttribute)

/**
 * Sets both read/write connection services
 *
 * @param string $connectionService
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, setConnectionService);

/**
 * Sets the DependencyInjection connection service used to write data
 *
 * @param string $connectionService
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, setWriteConnectionService);

/**
 * Sets the DependencyInjection connection service used to read data
 *
 * @param string $connectionService
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, setReadConnectionService);

/**
 * Returns DependencyInjection connection service used to read data
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getReadConnectionService);

/**
 * Returns DependencyInjection connection service used to write data
 *
 * @return string
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getWriteConnectionService);

/**
 * Gets internal database connection
 *
 * @return Dao\Db\AdapterInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getReadConnection);

/**
 * Gets internal database connection
 *
 * @return Dao\Db\AdapterInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getWriteConnection);

/**
 * Assigns values to a model from an array
 *
 * @param array $data
 * @param array $columnMap
 * @param array $whiteList
 * @return Dao\Mvc\Model
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, assign);

/**
 * Assigns values to a model from an array returning a new model
 *
 * @param Dao\Mvc\Model $base
 * @param array $data
 * @param array $columnMap
 * @param int $dirtyState
 * @param boolean $keepSnapshots
 * @return Dao\Mvc\Model $result
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, cloneResultMap);

/**
 * Assigns values to a model from an array returning a new model
 *
 * @param Dao\Mvc\Model $base
 * @param array $data
 * @param int $dirtyState
 * @return Dao\Mvc\Model
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, cloneResult);

/**
 * Returns an hydrated result based on the data and the column map
 *
 * @param Dao\Mvc\Model $base
 * @param array $data
 * @param array $columnMap
 * @param int $hydrationMode
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, cloneResultMapHydrate);

/**
 * Allows to query a set of records that match the specified conditions
 *
 * @param 	array $parameters
 * @return  Dao\Mvc\Model\ResultsetInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, find);

/**
 * Allows to query the first record that match the specified conditions
 *
 * @param array $parameters
 * @return Dao\Mvc\ModelInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, findFirst);

/**
 * Create a criteria for a especific model
 *
 * @param Dao\DiInterface $dependencyInjector
 * @return Dao\Mvc\Model\CriteriaInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, query);

/**
 * Allows to count how many records match the specified conditions
 *
 * @param array $parameters
 * @return int
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, count);

/**
 * Allows to calculate a summatory on a column that match the specified conditions
 *
 * @param array $parameters
 * @return double
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, sum);

/**
 * Allows to get the maximum value of a column that match the specified conditions
 *
 * @param array $parameters
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, maximum);

/**
 * Allows to get the minimum value of a column that match the specified conditions
 *
 * @param array $parameters
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, minimum);

/**
 * Allows to calculate the average value on a column matching the specified conditions
 *
 * @param array $parameters
 * @return double
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, average);

/**
 * Fires an event, implicitly calls behaviors and listeners in the events manager are notified
 *
 * @param string $eventName
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, fireEvent);

/**
 * Fires an event, implicitly calls behaviors and listeners in the events manager are notified
 * This method stops if one of the callbacks/listeners returns boolean false
 *
 * @param string $eventName
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, fireEventCancel);

/**
 * Appends a customized message on the validation process
 *
 * @param Dao\Mvc\Model\MessageInterface $message
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, appendMessage);

/**
 * Check whether validation process has generated any messages
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, validationHasFailed);

/**
 * Returns all the validation messages
 *
 * @return Dao\Mvc\Model\MessageInterface[]
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getMessages);

/**
 * Inserts or updates a model instance. Returning true on success or false otherwise.
 *
 * @param  array $data
 * @param  array $whiteList
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, save);

/**
 * Inserts a model instance. If the instance already exists in the persistance it will throw an exception
 * Returning true on success or false otherwise.
 *
 * @param  array $data
 * @param  array $whiteList
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, create);

/**
 * Updates a model instance. If the instance doesn't exist in the persistance it will throw an exception
 * Returning true on success or false otherwise.
 *
 * @param  array $data
 * @param  array $whiteList
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, update);

/**
 * Deletes a model instance. Returning true on success or false otherwise.
 *
 * @return boolean
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, delete);

/**
 * Returns the type of the latest operation performed by the ORM
 * Returns one of the OP_* class constants
 *
 * @return int
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getOperationMade);

/**
 * Refreshes the model attributes re-querying the record from the database
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, refresh);

/**
 * Reads an attribute value by its name
 *
 * @param string $attribute
 * @return mixed
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, readAttribute);

/**
 * Writes an attribute value by its name
 *
 * @param string $attribute
 * @param mixed $value
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, writeAttribute);

/**
 * Returns related records based on defined relations
 *
 * @param string $alias
 * @param array $arguments
 * @return Dao\Mvc\Model\ResultsetInterface
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, getRelated);

/**
 * Allows to delete a set of records that match the specified conditions
 *
 * @param 	array $parameters
 * @return	boolean
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, remove);

/*
 * Reset a model instance data
 */
DAO_DOC_METHOD(Dao_Mvc_ModelInterface, reset);
