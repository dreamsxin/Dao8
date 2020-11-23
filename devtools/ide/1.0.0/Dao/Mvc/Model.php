<?php 

namespace Dao\Mvc {

	/**
	 * Dao\Mvc\Model
	 *
	 * <p>Dao\Mvc\Model connects business objects and database tables to create
	 * a persistable domain model where logic and data are presented in one wrapping.
	 * It‘s an implementation of the object-relational mapping (ORM).</p>
	 *
	 * <p>A model represents the information (data) of the application and the rules to manipulate that data.
	 * Models are primarily used for managing the rules of interaction with a corresponding database table.
	 * In most cases, each table in your database will correspond to one model in your application.
	 * The bulk of your application’s business logic will be concentrated in the models.</p>
	 *
	 * <p>Dao\Mvc\Model is the first ORM written in C-language for PHP, giving to developers high performance
	 * when interacting with databases while is also easy to use.</p>
	 *
	 * <code>
	 *
	 * $robot = new Robots();
	 * $robot->type = 'mechanical';
	 * $robot->name = 'Astro Boy';
	 * $robot->year = 1952;
	 * if ($robot->save() == false) {
	 *  echo "Umh, We can store robots: ";
	 *  foreach ($robot->getMessages() as $message) {
	 *    echo $message;
	 *  }
	 * } else {
	 *  echo "Great, a new robot was saved successfully!";
	 * }
	 * </code>
	 *
	 */
	
	abstract class Model extends \Dao\Di\Injectable implements \Dao\Events\EventsAwareInterface, \Dao\Di\InjectionAwareInterface, \Dao\Mvc\ModelInterface, \Dao\Mvc\Model\ResultInterface, \Serializable, \JsonSerializable {

		const OP_NONE = 0;

		const OP_CREATE = 1;

		const OP_UPDATE = 2;

		const OP_DELETE = 3;

		const DIRTY_STATE_PERSISTENT = 0;

		const DIRTY_STATE_TRANSIENT = 1;

		const DIRTY_STATE_DETACHED = 2;

		protected $_errorMessages;

		protected $_operationMade;

		protected $_dirtyState;

		protected $_transaction;

		protected $_uniqueKey;

		protected $_uniqueParams;

		protected $_uniqueTypes;

		protected $_skipped;

		protected $_related;

		protected $_snapshot;

		protected $_seenRawvalues;

		protected $_filter;

		protected $_relatedResult;

		protected $_columnMap;

		/**
		 * register model class
		 *
		 * @param string $model
		 * @param string $table
		 * @return boolean
		 */
		public static function register($model, $table=null){ }


		/**
		 * \Dao\Mvc\Model constructor
		 *
		 * @param \Dao\DiInterface $dependencyInjector
		 * @param \Dao\Mvc\Model\ManagerInterface $modelsManager
		 */
		final public function __construct($data=null, $dependencyInjector=null){ }


		/**
		 * Sets a custom events manager
		 *
		 * @param \Dao\Events\ManagerInterface $eventsManager
		 */
		public function setEventsManager(\Dao\Events\ManagerInterface $eventsManager){ }


		/**
		 * Returns the custom events manager
		 *
		 * @return \Dao\Events\ManagerInterface
		 */
		public function getEventsManager(){ }


		/**
		 * Returns the models meta-data service related to the entity instance
		 *
		 * @return \Dao\Mvc\Model\MetaDataInterface
		 */
		public function getModelsMetaData(){ }


		/**
		 * Returns the models manager related to the entity instance
		 *
		 * @return \Dao\Mvc\Model\ManagerInterface
		 */
		public function getModelsManager(){ }


		/**
		 * Sets a transaction related to the Model instance
		 *
		 *<code>
		 *use \Dao\Mvc\Model\Transaction\Manager as TxManager;
		 *use \Dao\Mvc\Model\Transaction\Failed as TxFailed;
		 *
		 *try {
		 *
		 *  $txManager = new TxManager();
		 *
		 *  $transaction = $txManager->get();
		 *
		 *  $robot = new Robots();
		 *  $robot->setTransaction($transaction);
		 *  $robot->name = 'WALL·E';
		 *  $robot->created_at = date('Y-m-d');
		 *  if ($robot->save() == false) {
		 *    $transaction->rollback("Can't save robot");
		 *  }
		 *
		 *  $robotPart = new RobotParts();
		 *  $robotPart->setTransaction($transaction);
		 *  $robotPart->type = 'head';
		 *  if ($robotPart->save() == false) {
		 *    $transaction->rollback("Robot part cannot be saved");
		 *  }
		 *
		 *  $transaction->commit();
		 *
		 *} catch (TxFailed $e) {
		 *  echo 'Failed, reason: ', $e->getMessage();
		 *}
		 *
		 *</code>
		 *
		 * @param \Dao\Mvc\Model\TransactionInterface $transaction
		 * @return \Dao\Mvc\Model
		 */
		public function setTransaction($transaction){ }


		/**
		 * Returns a transaction related in the Model instance
		 *
		 * @return \Dao\Mvc\Model\TransactionInterface
		 */
		public function getTransaction(){ }


		/**
		 * Sets table name which model should be mapped
		 *
		 * @param string $source
		 * @return \Dao\Mvc\Model
		 */
		public function setSource($source){ }


		/**
		 * Returns table name mapped in the model
		 *
		 * @param \Dao\Mvc\Model\Query $query
		 * @return string
		 */
		public function getSource(){ }


		/**
		 * Sets schema name where table mapped is located
		 *
		 * @param string $schema
		 * @return \Dao\Mvc\Model
		 */
		public function setSchema($schema){ }


		/**
		 * Returns schema name where table mapped is located
		 *
		 * @param \Dao\Mvc\Model\Query $query
		 * @return string
		 */
		public function getSchema(){ }


		/**
		 * Returns the name of identity field (if one is present)
		 *
		 * @return string
		 */
		public function getIdentityField(){ }


		/**
		 * Returns the column map if any
		 *
		 * @return array
		 */
		public function getColumnMap(){ }


		/**
		 * Returns the reverse column map if any
		 *
		 * @return array
		 */
		public function getReverseColumnMap(){ }


		/**
		 * Returns table attributes names (fields)
		 *
		 * @return array
		 */
		public function getAttributes(){ }


		/**
		 * Returns an array of fields which are part of the primary key
		 *
		 * @return array
		 */
		public function getPrimaryKeyAttributes(){ }


		/**
		 * Returns an arrau of fields which are not part of the primary key
		 *
		 * @return array
		 */
		public function getNonPrimaryKeyAttributes(){ }


		/**
		 * Returns an array of not null attributes
		 *
		 * @return array
		 */
		public function getNotNullAttributes(){ }


		/**
		 * Returns attributes which types are numerical
		 *
		 * @return array
		 */
		public function getDataTypesNumeric(){ }


		/**
		 * Checks if the attribute is not null
		 *
		 * @return array
		 */
		public function isNotNull($attribute){ }


		/**
		 * Returns the columns data types
		 *
		 * @return array
		 */
		public function getDataTypes(){ }


		/**
		 * Returns attribute data size
		 *
		 * @param string $attribute
		 * @return array
		 */
		public function getDataSize($attribute){ }


		/**
		 * Returns attribute data byte
		 *
		 * @param string $attribute
		 * @return array
		 */
		public function getDataByte($attribute){ }


		/**
		 * Returns attribute data scale
		 *
		 * @param string $attribute
		 * @return array
		 */
		public function getDataScale($attribute){ }


		/**
		 * Returns attributes and their bind data types
		 *
		 * @return array
		 */
		public function getBindTypes(){ }


		/**
		 * Returns attributes and their default values
		 *
		 * @return array
		 */
		public function getDefaultValues(){ }


		/**
		 * Returns attributes that must be ignored from the INSERT SQL generation
		 *
		 * @return array
		 */
		public function getAutomaticCreateAttributes(){ }


		/**
		 * Returns attributes that must be ignored from the UPDATE SQL generation
		 *
		 * @return array
		 */
		public function getAutomaticUpdateAttributes(){ }


		/**
		 * Check if a model has certain column
		 *
		 * @param string $column
		 * @return boolean
		 */
		public function hasRealAttribute($column){ }


		/**
		 * Gets a model certain column
		 *
		 * @param string $column
		 * @return string
		 */
		public function getRealAttribute($column){ }


		/**
		 * Check if a model has certain attribute
		 *
		 * @param string $attribute
		 * @return boolean
		 */
		public function hasAttribute($attribute){ }


		/**
		 * Gets a model certain attribute
		 *
		 * @param string $attribute
		 * @return string
		 */
		public function getAttribute($attribute){ }


		/**
		 * Sets a model field value
		 *
		 * @param string $attribute
		 * @param mixed
		 * @return \Dao\Mvc\Model
		 */
		public function setFieldValue($attribute, $value){ }


		/**
		 * Gets a model field value
		 *
		 * @param string $attribute
		 * @return mixed
		 */
		public function getFieldValue($attribute){ }


		/**
		 * Sets the DependencyInjection connection service name
		 *
		 * @param string $connectionService
		 * @return \Dao\Mvc\Model
		 */
		public function setConnectionService($connectionService){ }


		/**
		 * Sets the DependencyInjection connection service name used to read data
		 *
		 * @param string $connectionService
		 * @return \Dao\Mvc\Model
		 */
		public function setReadConnectionService($connectionService){ }


		/**
		 * Sets the DependencyInjection connection service name used to write data
		 *
		 * @param string $connectionService
		 * @return \Dao\Mvc\Model
		 */
		public function setWriteConnectionService($connectionService){ }


		/**
		 * Returns the DependencyInjection connection service name used to read data related the model
		 *
		 * @return string
		 */
		public function getReadConnectionService(){ }


		/**
		 * Returns the DependencyInjection connection service name used to write data related to the model
		 *
		 * @return string
		 */
		public function getWriteConnectionService(){ }


		/**
		 * Sets the dirty state of the object using one of the DIRTY_STATE_* constants
		 *
		 * @param int $dirtyState
		 * @return \Dao\Mvc\Model
		 */
		public function setDirtyState($dirtyState){ }


		/**
		 * Returns one of the DIRTY_STATE_* constants telling if the record exists in the database or not
		 *
		 * @return int
		 */
		public function getDirtyState(){ }


		/**
		 * Gets the connection used to read data for the model
		 *
		 * @param \Dao\Mvc\Model\Query $query
		 * @param array $intermediate
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @return \Dao\Db\AdapterInterface
		 */
		public function getReadConnection(){ }


		/**
		 * Gets the connection used to write data to the model
		 *
		 * @param \Dao\Mvc\Model\Query $query
		 * @param array $intermediate
		 * @param array $bindParams
		 * @param array $bindTypes
		 * @return \Dao\Db\AdapterInterface
		 */
		public function getWriteConnection(){ }


		/**
		 * Assigns values to a model from an array
		 *
		 *<code>
		 *$robot->assign(array(
		 *  'type' => 'mechanical',
		 *  'name' => 'Astro Boy',
		 *  'year' => 1952
		 *));
		 *</code>
		 *
		 * @param array $data
		 * @param array $columnMap
		 * @param array $whiteList
		 * @return \Dao\Mvc\Model
		 */
		public function assign($data, $columnMap=null, $whiteList=null, $negate=null){ }


		/**
		 * Assigns values to a model from an array returning a new model.
		 *
		 *<code>
		 *$robot = \Dao\Mvc\Model::cloneResultMap(new Robots(), array(
		 *  'type' => 'mechanical',
		 *  'name' => 'Astro Boy',
		 *  'year' => 1952
		 *));
		 *</code>
		 *
		 * @param \Dao\Mvc\Model $base
		 * @param array $data
		 * @param array $columnMap
		 * @param int $dirtyState
		 * @param boolean $keepSnapshots
		 * @param \Dao\Mvc\Model $sourceModel
		 * @return \Dao\Mvc\Model
		 */
		public static function cloneResultMap($base, $data, $columnMap, $dirtyState, $keepSnapshots=null, $sourceModel=null){ }


		/**
		 * Returns an hydrated result based on the data and the column map
		 *
		 * @param array $data
		 * @param array $columnMap
		 * @param int $hydrationMode
		 * @param \Dao\Mvc\Model $sourceModel
		 * @return mixed
		 */
		public static function cloneResultMapHydrate($data, $columnMap, $hydrationMode, $sourceModel=null){ }


		/**
		 * Assigns values to a model from an array returning a new model
		 *
		 *<code>
		 *$robot = \Dao\Mvc\Model::cloneResult(new Robots(), array(
		 *  'type' => 'mechanical',
		 *  'name' => 'Astro Boy',
		 *  'year' => 1952
		 *));
		 *</code>
		 *
		 * @param \Dao\Mvc\Model $base
		 * @param array $data
		 * @param int $dirtyState
		 * @return \Dao\Mvc\Model
		 */
		public static function cloneResult($base, $data, $dirtyState=null, $sourceModel=null){ }


		/**
		 * Allows to query a set of records that match the specified conditions
		 *
		 * <code>
		 *
		 * //How many robots are there?
		 * $robots = Robots::find();
		 * echo "There are ", count($robots), "\n";
		 *
		 * //How many mechanical robots are there?
		 * $robots = Robots::find("type='mechanical'");
		 * echo "There are ", count($robots), "\n";
		 *
		 * //Get and print virtual robots ordered by name
		 * $robots = Robots::find(array("type='virtual'", "order" => "name"));
		 * foreach ($robots as $robot) {
		 *	   echo $robot->name, "\n";
		 * }
		 *
		 * $robots = Robots::find("type=:type:", array("type" => "virtual"), array("order" => "name"));
		 * foreach ($robots as $robot) {
		 *	   echo $robot->name, "\n";
		 * }
		 *
		 * //Get first 100 virtual robots ordered by name
		 * $robots = Robots::find(array("type='virtual'", "order" => "name", "limit" => 100));
		 * foreach ($robots as $robot) {
		 *	   echo $robot->name, "\n";
		 * }
		 * </code>
		 *
		 * @param string|array $conditions
		 * @param array $bindParams
		 * @param array $options
		 * @return  \Dao\Mvc\Model\ResultsetInterface
		 */
		public static function find($conditions=null, $bindParams=null, $options=null){ }


		/**
		 * Allows to query the first record that match the specified conditions
		 *
		 * <code>
		 *
		 * //What's the first robot in robots table?
		 * $robot = Robots::findFirst();
		 * echo "The robot name is ", $robot->name;
		 *
		 * //What's the first mechanical robot in robots table?
		 * $robot = Robots::findFirst("type='mechanical'");
		 * echo "The first mechanical robot name is ", $robot->name;
		 *
		 * //Get first virtual robot ordered by name
		 * $robot = Robots::findFirst(array("type='virtual'", "order" => "name"));
		 * echo "The first virtual robot name is ", $robot->name;
		 *
		 * </code>
		 *
		 * @param string|array $conditions
		 * @param array $bindParams
		 * @param array $options
		 * @param bool $autoCreate
		 * @return \Dao\Mvc\Model
		 */
		public static function findFirst($conditions=null, $bindParams=null, $options=null, $autoCreate=null){ }


		/**
		 * Create a criteria for a specific model
		 *
		 * @param \Dao\DiInterface $dependencyInjector
		 * @return \Dao\Mvc\Model\Criteria
		 */
		public static function query(\Dao\DiInterface $dependencyInjector=null){ }


		/**
		 * Builds a unique primary key condition
		 *
		 * @return boolean
		 */
		public function build(){ }


		/**
		 * Gets a unique key
		 *
		 * @return string
		 */
		public function getUniqueKey(){ }


		/**
		 * Gets a unique params
		 *
		 * @return array
		 */
		public function getUniqueParams(){ }


		/**
		 * Gets a unique params
		 *
		 * @return array
		 */
		public function getUniqueTypes(){ }


		/**
		 * Checks if the current record already exists or not
		 *
		 * @param \Dao\Mvc\Model\MetadataInterface $metaData
		 * @param \Dao\Db\AdapterInterface $connection
		 * @return boolean
		 */
		public function exists(){ }


		/**
		 * Builds a unique primary key condition
		 *
		 * @return boolean
		 */
		protected function _reBuild(){ }


		/**
		 * Generate a PHQL SELECT statement for an aggregate
		 *
		 *<code>
		 *
		 *	Robots::group(['aggregators' => array(array('column' => 'id', 'aggregator' => 'sum'), 'sumatory' => array('column' => 'price', 'aggregator' => 'sum'))]);
		 *
		 *</code>
		 *
		 * @param array $parameters
		 * @return \Dao\Mvc\Model\ResultsetInterface
		 */
		public static function group($params){ }


		/**
		 * Allows to count how many records match the specified conditions
		 *
		 * <code>
		 *
		 * //How many robots are there?
		 * $number = Robots::count();
		 * echo "There are ", $number, "\n";
		 *
		 * //How many mechanical robots are there?
		 * $number = Robots::count("type='mechanical'");
		 * echo "There are ", $number, " mechanical robots\n";
		 *
		 * </code>
		 *
		 * @param array $parameters
		 * @return int
		 */
		public static function count($parameters=null){ }


		/**
		 * Allows to calculate a summatory on a column that match the specified conditions
		 *
		 * <code>
		 *
		 * //How much are all robots?
		 * $sum = Robots::sum(array('column' => 'price'));
		 * echo "The total price of robots is ", $sum, "\n";
		 *
		 * //How much are mechanical robots?
		 * $sum = Robots::sum(array("type='mechanical'", 'column' => 'price'));
		 * echo "The total price of mechanical robots is  ", $sum, "\n";
		 *
		 * </code>
		 *
		 * @param array $parameters
		 * @return double
		 */
		public static function sum($parameters=null){ }


		/**
		 * Allows to get the maximum value of a column that match the specified conditions
		 *
		 * <code>
		 *
		 * //What is the maximum robot id?
		 * $id = Robots::maximum(array('column' => 'id'));
		 * echo "The maximum robot id is: ", $id, "\n";
		 *
		 * //What is the maximum id of mechanical robots?
		 * $sum = Robots::maximum(array("type='mechanical'", 'column' => 'id'));
		 * echo "The maximum robot id of mechanical robots is ", $id, "\n";
		 *
		 * </code>
		 *
		 * @param array $parameters
		 * @return mixed
		 */
		public static function maximum($parameters=null){ }


		/**
		 * Allows to get the minimum value of a column that match the specified conditions
		 *
		 * <code>
		 *
		 * //What is the minimum robot id?
		 * $id = Robots::minimum(array('column' => 'id'));
		 * echo "The minimum robot id is: ", $id;
		 *
		 * //What is the minimum id of mechanical robots?
		 * $sum = Robots::minimum(array("type='mechanical'", 'column' => 'id'));
		 * echo "The minimum robot id of mechanical robots is ", $id;
		 *
		 * </code>
		 *
		 * @param array $parameters
		 * @return mixed
		 */
		public static function minimum($parameters=null){ }


		/**
		 * Allows to calculate the average value on a column matching the specified conditions
		 *
		 * <code>
		 *
		 * //What's the average price of robots?
		 * $average = Robots::average(array('column' => 'price'));
		 * echo "The average price is ", $average, "\n";
		 *
		 * //What's the average price of mechanical robots?
		 * $average = Robots::average(array("type='mechanical'", 'column' => 'price'));
		 * echo "The average price of mechanical robots is ", $average, "\n";
		 *
		 * </code>
		 *
		 * @param array $parameters
		 * @return double
		 */
		public static function average($parameters=null){ }


		/**
		 * Fires an event, implicitly calls behaviors and listeners in the events manager are notified
		 *
		 * @param string $eventName
		 * @param mixed $data
		 * @return mixed
		 */
		public function fireEvent($eventName, $data=null, $cancelable=null){ }


		/**
		 * Fires an event, implicitly calls behaviors and listeners in the events manager are notified
		 * This method stops if one of the callbacks/listeners returns boolean false
		 *
		 * @param string $eventName
		 * @param mixed $data
		 * @param boolean $cancelAble
		 * @return boolean
		 */
		public function fireEventCancel($eventName, $data=null, $cancelable=null){ }


		/**
		 * Cancel the current operation
		 *
		 * @return boolean
		 */
		protected function _cancelOperation(){ }


		/**
		 * Appends a customized message on the validation process
		 *
		 * <code>
		 * use \Dao\Validation\Message as Message;
		 *
		 * class Robots extends \Dao\Mvc\Model
		 * {
		 *
		 *   public function beforeSave()
		 *   {
		 *     if ($this->name == 'Peter') {
		 *        $message = new Message("Sorry, but a robot cannot be named Peter");
		 *        $this->appendMessage($message);
		 *     }
		 *   }
		 * }
		 * </code>
		 *
		 * @param \Dao\Validation\MessageInterface $message
		 * @return \Dao\Mvc\Model
		 */
		public function appendMessage($message, $field=null, $type=null, $code=null){ }


		/**
		 * Executes validators on every validation call
		 *
		 *<code>
		 *use \Dao\Mvc\Model\Validator\ExclusionIn as ExclusionIn;
		 *
		 *class Subscriptors extends \Dao\Mvc\Model
		 *{
		 *
		 *	public function validation()
		 *  {
		 * 		$validation = new \Dao\Validation();
		 * 		$validation->add('status', new ExclusionIn(array(
		 *			'domain' => array('A', 'I')
		 *		)));
		 * 		return $this->validate($validation);
		 *	}
		 *
		 *}
		 *</code>
		 *
		 * @param array|Dao\ValidationInterface $validation
		 * @return \Dao\Mvc\Model
		 */
		protected function validate($validation){ }


		/**
		 * Check whether validation process has generated any messages
		 *
		 *<code>
		 *use \Dao\Mvc\Model\Validator\ExclusionIn as ExclusionIn;
		 *
		 *class Subscriptors extends \Dao\Mvc\Model
		 *{
		 *
		 *	public function validation()
		 *  {
		 * 		$this->validate(new ExclusionIn(array(
		 *			'field' => 'status',
		 *			'domain' => array('A', 'I')
		 *		)));
		 *		if ($this->validationHasFailed() == true) {
		 *			return false;
		 *		}
		 *	}
		 *
		 *}
		 *</code>
		 *
		 * @return boolean
		 */
		public function validationHasFailed(){ }


		/**
		 * Returns all the validation messages
		 *
		 *<code>
		 *	$robot = new Robots();
		 *	$robot->type = 'mechanical';
		 *	$robot->name = 'Astro Boy';
		 *	$robot->year = 1952;
		 *	if ($robot->save() == false) {
		 *  	echo "Umh, We can't store robots right now ";
		 *  	foreach ($robot->getMessages() as $message) {
		 *			echo $message;
		 *		}
		 *	} else {
		 *  	echo "Great, a new robot was saved successfully!";
		 *	}
		 * </code>
		 *
		 * @return \Dao\Mvc\Model\MessageInterface[]
		 */
		public function getMessages($filter=null){ }


		/**
		 * Reads "belongs to" relations and check the virtual foreign keys when inserting or updating records
		 * to verify that inserted/updated values are present in the related entity
		 *
		 * @return boolean
		 */
		protected function _checkForeignKeysRestrict(){ }


		/**
		 * Reads both "hasMany" and "hasOne" relations and checks the virtual foreign keys (restrict) when deleting records
		 *
		 * @return boolean
		 */
		protected function _checkForeignKeysReverseRestrict(){ }


		/**
		 * Reads both "hasMany" and "hasOne" relations and checks the virtual foreign keys (cascade) when deleting records
		 *
		 * @return boolean
		 */
		protected function _checkForeignKeysReverseCascade(){ }


		/**
		 * Executes internal hooks before save a record
		 *
		 * @param \Dao\Mvc\Model\MetadataInterface $metaData
		 * @param boolean $exists
		 * @param string $identityField
		 * @return boolean
		 */
		protected function _preSave(){ }


		/**
		 * Executes internal events after save a record
		 *
		 * @param boolean $success
		 * @param boolean $exists
		 * @return boolean
		 */
		protected function _postSave(){ }


		/**
		 * Sends a pre-build INSERT SQL statement to the relational database system
		 *
		 * @param \Dao\Mvc\Model\MetadataInterface $metaData
		 * @param \Dao\Db\AdapterInterface $connection
		 * @param string $table
		 * @return boolean
		 */
		protected function _doLowInsert(){ }


		/**
		 * Sends a pre-build UPDATE SQL statement to the relational database system
		 *
		 * @param \Dao\Mvc\Model\MetadataInterface $metaData
		 * @param \Dao\Db\AdapterInterface $connection
		 * @param string|array $table
		 * @return boolean|int
		 */
		protected function _doLowUpdate(){ }


		/**
		 * Saves related records that must be stored prior to save the master record
		 *
		 * @param \Dao\Db\AdapterInterface $connection
		 * @param \Dao\Mvc\ModelInterface[] $related
		 * @return boolean
		 */
		protected function _preSaveRelatedRecords(){ }


		/**
		 * Save the related records assigned in the has-one/has-many relations
		 *
		 * @param \Dao\Db\AdapterInterface $connection
		 * @param \Dao\Mvc\ModelInterface[] $related
		 * @return boolean
		 */
		protected function _postSaveRelatedRecords(){ }


		/**
		 * Inserts or updates a model instance. Returning true on success or false otherwise.
		 *
		 *<code>
		 *	//Creating a new robot
		 *	$robot = new Robots();
		 *	$robot->type = 'mechanical';
		 *	$robot->name = 'Astro Boy';
		 *	$robot->year = 1952;
		 *	$robot->save();
		 *
		 *	//Updating a robot name
		 *	$robot = Robots::findFirst("id=100");
		 *	$robot->name = "Biomass";
		 *	$robot->save();
		 *</code>
		 *
		 * @param array $data
		 * @param array $whiteList
		 * @param array $exists
		 * @param array $existsCheck
		 * @return boolean
		 */
		public function save($data=null, $whiteList=null, $exists=null){ }


		/**
		 * Inserts a model instance. If the instance already exists in the persistance it will throw an exception
		 * Returning true on success or false otherwise.
		 *
		 *<code>
		 *	//Creating a new robot
		 *	$robot = new Robots();
		 *	$robot->type = 'mechanical';
		 *	$robot->name = 'Astro Boy';
		 *	$robot->year = 1952;
		 *	$robot->create();
		 *
		 *  //Passing an array to create
		 *  $robot = new Robots();
		 *  $robot->create(array(
		 *      'type' => 'mechanical',
		 *      'name' => 'Astroy Boy',
		 *      'year' => 1952
		 *  ));
		 *</code>
		 *
		 * @param array $data
		 * @param array $whiteList
		 * @param boolean $existsCheck
		 * @return boolean
		 */
		public function create($data=null, $whiteList=null, $existsCheck=null){ }


		/**
		 * Updates a model instance. If the instance doesn't exist in the persistance it will throw an exception
		 * Returning true on success or false otherwise.
		 *
		 *<code>
		 *	//Updating a robot name
		 *	$robot = Robots::findFirst("id=100");
		 *	$robot->name = "Biomass";
		 *	$robot->update();
		 *</code>
		 *
		 * @param array $data
		 * @param array $whiteList
		 * @param boolean $existsCheck
		 * @return boolean
		 */
		public function update($data=null, $whiteList=null, $existsCheck=null){ }


		/**
		 * Deletes a model instance. Returning true on success or false otherwise.
		 *
		 * <code>
		 *$robot = Robots::findFirst("id=100");
		 *$robot->delete();
		 *
		 *foreach (Robots::find("type = 'mechanical'") as $robot) {
		 *   $robot->delete();
		 *}
		 * </code>
		 *
		 * @return boolean
		 */
		public function delete(){ }


		/**
		 * Returns the type of the latest operation performed by the ORM
		 * Returns one of the OP_* class constants
		 *
		 * @return int
		 */
		public function getOperationMade(){ }


		/**
		 * Refreshes the model attributes re-querying the record from the database
		 */
		public function refresh(){ }


		/**
		 * Skips the current operation forcing a success state
		 *
		 * @param boolean $skip
		 */
		public function skipOperation($skip){ }


		/**
		 * Reads an attribute value by its name
		 *
		 * <code>
		 * echo $robot->readAttribute('name');
		 * </code>
		 *
		 * @param string $attribute
		 * @return mixed
		 */
		public function readAttribute($attribute){ }


		/**
		 * Writes an attribute value by its name
		 *
		 * <code>
		 * 	$robot->writeAttribute('name', 'Rosey');
		 * </code>
		 *
		 * @param string $attribute
		 * @param mixed $value
		 */
		public function writeAttribute($attribute, $value){ }


		/**
		 * Sets a list of attributes that must be skipped from the
		 * generated INSERT/UPDATE statement
		 *
		 *<code>
		 *
		 *class Robots extends \Dao\Mvc\Model
		 *{
		 *
		 *   public function initialize()
		 *   {
		 *       $this->skipAttributes(array('price'));
		 *   }
		 *
		 *}
		 *</code>
		 *
		 * @param array $attributes
		 * @param boolean $replace
		 */
		protected function skipAttributes($attributes, $replace=null){ }


		/**
		 * Sets a list of attributes that must be skipped from the
		 * generated INSERT statement
		 *
		 *<code>
		 *
		 *class Robots extends \Dao\Mvc\Model
		 *{
		 *
		 *   public function initialize()
		 *   {
		 *       $this->skipAttributesOnCreate(array('created_at'));
		 *   }
		 *
		 *}
		 *</code>
		 *
		 * @param array $attributes
		 * @param boolean $replace
		 */
		protected function skipAttributesOnCreate($attributes, $replace=null){ }


		/**
		 * Returns attributes that must be ignored from the INSERT SQL generation
		 *
		 *<code>
		 * $robot = Robots::findFirst();
		 * print_r($robot->getSkipAttributesOnCreate());
		 *</code>
		 *
		 * @return array
		 */
		public function getSkipAttributesOnCreate(){ }


		/**
		 * Sets a list of attributes that must be skipped from the
		 * generated UPDATE statement
		 *
		 *<code>
		 *
		 *class Robots extends \Dao\Mvc\Model
		 *{
		 *
		 *   public function initialize()
		 *   {
		 *       $this->skipAttributesOnUpdate(array('modified_in'));
		 *   }
		 *
		 *}
		 *</code>
		 *
		 * @param array $attributes
		 * @param boolean $replace
		 */
		protected function skipAttributesOnUpdate($attributes, $replace=null){ }


		/**
		 * Returns attributes that must be ignored from the UPDATE SQL generation
		 *
		 *<code>
		 * $robot = Robots::findFirst();
		 * print_r($robot->getSkipAttributesOnUpdate());
		 *</code>
		 *
		 * @return array
		 */
		public function getSkipAttributesOnUpdate(){ }


		/**
		 * Setup a 1-1 relation between two models
		 *
		 *<code>
		 *
		 *class Robots extends \Dao\Mvc\Model
		 *{
		 *
		 *   public function initialize()
		 *   {
		 *       $this->hasOne('id', 'RobotsDescription', 'robots_id');
		 *   }
		 *
		 *}
		 *</code>
		 *
		 * @param mixed $fields
		 * @param string $referenceModel
		 * @param mixed $referencedFields
		 * @param   array $options
		 * @return  \Dao\Mvc\Model\Relation
		 */
		public function hasOne($fields, $referenceModel, $referencedFields, $options=null){ }


		/**
		 * Setup a relation reverse 1-1  between two models
		 *
		 *<code>
		 *
		 *class RobotsParts extends \Dao\Mvc\Model
		 *{
		 *
		 *   public function initialize()
		 *   {
		 *       $this->belongsTo('robots_id', 'Robots', 'id');
		 *   }
		 *
		 *}
		 *</code>
		 *
		 * @param mixed $fields
		 * @param string $referenceModel
		 * @param mixed $referencedFields
		 * @param   array $options
		 * @return  \Dao\Mvc\Model\Relation
		 */
		public function belongsTo($fields, $referenceModel, $referencedFields, $options=null){ }


		/**
		 * Setup a relation 1-n between two models
		 *
		 *<code>
		 *
		 *class Robots extends \Dao\Mvc\Model
		 *{
		 *
		 *   public function initialize()
		 *   {
		 *       $this->hasMany('id', 'RobotsParts', 'robots_id');
		 *   }
		 *
		 *}
		 *</code>
		 *
		 * @param mixed $fields
		 * @param string $referenceModel
		 * @param mixed $referencedFields
		 * @param   array $options
		 * @return  \Dao\Mvc\Model\Relation
		 */
		public function hasMany($fields, $referenceModel, $referencedFields, $options=null){ }


		/**
		 * Setup a relation n-n between two models through an intermediate relation
		 *
		 *<code>
		 *
		 *class Robots extends \Dao\Mvc\Model
		 *{
		 *
		 *   public function initialize()
		 *   {
		 *       //Setup a many-to-many relation to Parts through RobotsParts
		 *       $this->hasManyToMany(
		 *			'id',
		 *			'RobotsParts',
		 *			'robots_id',
		 *			'parts_id',
		 *			'Parts',
		 *			'id'
		 *		);
		 *   }
		 *
		 *}
		 *</code>
		 *
		 * @param string $fields
		 * @param string $intermediateModel
		 * @param string $intermediateFields
		 * @param string $intermediateReferencedFields
		 * @param string $referencedModel
		 * @param   string $referencedFields
		 * @param   array $options
		 * @return  \Dao\Mvc\Model\Relation
		 */
		public function hasManyToMany($fields, $intermediateModel, $intermediateFields, $intermediateReferencedFields, $referenceModel, $referencedFields, $options=null){ }


		/**
		 * Setups a behavior in a model
		 *
		 *<code>
		 *
		 *use \Dao\Mvc\Model\Behavior\Timestampable;
		 *
		 *class Robots extends \Dao\Mvc\Model
		 *{
		 *
		 *   public function initialize()
		 *   {
		 *		$this->addBehavior(new Timestampable(array(
		 *			'onCreate' => array(
		 *				'field' => 'created_at',
		 *				'format' => 'Y-m-d'
		 *			)
		 *		)));
		 *   }
		 *
		 *}
		 *</code>
		 *
		 * @param \Dao\Mvc\Model\BehaviorInterface $behavior
		 */
		public function addBehavior($behavior){ }


		/**
		 * Sets the record's snapshot data.
		 * This method is used internally to set snapshot data when the model was set up to keep snapshot data
		 *
		 * @param array $data
		 * @param array $columnMap
		 */
		public function setSnapshotData($data, $columnMap=null){ }


		/**
		 * Checks if the object has internal snapshot data
		 *
		 * @return boolean
		 */
		public function hasSnapshotData(){ }


		/**
		 * Returns the internal snapshot data
		 *
		 * @return array
		 */
		public function getSnapshotData(){ }


		/**
		 * Check if a specific attribute has changed
		 * This only works if the model is keeping data snapshots
		 *
		 * @param string $fieldName
		 * @return boolean
		 */
		public function hasChanged($fieldName=null){ }


		/**
		 * Returns a list of changed values
		 *
		 * @return array
		 */
		public function getChangedFields(){ }


		/**
		 * Sets if a model must use dynamic update instead of the all-field update
		 *
		 *<code>
		 *
		 *class Robots extends \Dao\Mvc\Model
		 *{
		 *
		 *   public function initialize()
		 *   {
		 *		$this->useDynamicUpdate(true);
		 *   }
		 *
		 *}
		 *</code>
		 *
		 * @param boolean $dynamicUpdate
		 */
		protected function useDynamicUpdate($dynamicUpdate){ }


		/**
		 * Returns related records based on defined relations
		 *
		 * @param string $alias
		 * @param array $arguments
		 * @return \Dao\Mvc\Model\ResultsetInterface
		 */
		public function getRelated($alias, $arguments=null){ }


		/**
		 * Returns related records defined relations depending on the method name
		 *
		 * @param string $modelName
		 * @param string $method
		 * @param array $arguments
		 * @return mixed
		 */
		protected function _getRelatedRecords(){ }


		/**
		 * Handles method calls when a method is not implemented
		 *
		 * @param string $method
		 * @param array $arguments
		 * @return mixed
		 */
		public function __call($method, $arguments=null){ }


		/**
		 * Handles method calls when a static method is not implemented
		 *
		 * @param string $method
		 * @param array $arguments
		 * @return mixed
		 */
		public static function __callStatic($method, $arguments=null){ }


		/**
		 * Magic method to assign values to the the model
		 *
		 * @param string $property
		 * @param mixed $value
		 */
		public function __set($property, $value){ }


		/**
		 * Magic method to get related records using the relation alias as a property
		 *
		 * @param string $property
		 * @return \Dao\Mvc\Model\Resultset
		 */
		public function __get($property){ }


		/**
		 * Magic method to check if a property is a valid relation
		 *
		 * @param string $property
		 */
		public function __isset($property){ }


		/**
		 * Serializes the object ignoring connections, services, related objects or static properties
		 *
		 * @return string
		 */
		public function serialize(){ }


		/**
		 * Unserializes the object from a serialized string
		 *
		 * @param string $data
		 */
		public function unserialize($data){ }


		/**
		 * Returns a simple representation of the object that can be used with var_dump
		 *
		 *<code>
		 * var_dump($robot->dump());
		 *</code>
		 *
		 * @return array
		 */
		public function dump(){ }


		public function getFieldValues(){ }


		/**
		 * Returns the instance as an array representation
		 *
		 *<code>
		 * print_r($robot->toArray());
		 *</code>
		 *
		 * @param array $columns
		 * @param bool $mustColumn
		 * @param bool $negate
		 * @return array
		 */
		public function toArray($columns=null, $mustColumn=null, $negate=null){ }


		/**
		 * Enables/disables options in the ORM
		 * Available options:
		 * events                — Enables/Disables globally the internal events
		 * virtualForeignKeys    — Enables/Disables virtual foreign keys
		 * columnRenaming        — Enables/Disables column renaming
		 * notNullValidations    — Enables/Disables automatic not null validation
		 * exceptionOnFailedSave — Enables/Disables throws an exception if the saving process fails
		 * phqlLiterals          — Enables/Disables literals in PHQL this improves the security of applications
		 * propertyMethod        — Enables/Disables property method
		 * autoConvert           — Enables/Disables auto convert
		 * strict                — Enables/Disables strict mode
		 * strict                — Enables/Disables strict mode
		 *
		 * @param array $options
		 */
		public static function setup($options){ }


		/**
		 * Allows to delete a set of records that match the specified conditions
		 *
		 * <code>
		 * $robot = Robots::remove("id=100")
		 * </code>
		 *
		 * @param 	array $parameters
		 * @return	boolean
		 */
		public static function remove($parameters){ }


		public function reset(){ }


		/**
		 * Sanitizes a value with a specified single or set of filters
		 *
		 *<code>
		 *use \Dao\Mvc\Model\Validator\ExclusionIn as ExclusionIn;
		 *
		 *class Subscriptors extends \Dao\Mvc\Model
		 *{
		 *
		 *	public function filters()
		 *  {
		 * 		$this->filter('status', 'int');
		 *	}
		 *
		 *}
		 *</code>
		 *
		 * @param string $field
		 * @param string|array $filters
		 * @param mixed $defaultValue
		 * @param boolean $notAllowEmpty
		 * @param boolean $recursive
		 * @return \Dao\Mvc\Model
		 */
		protected function filter($field, $filters, $defaultValue=null, $notAllowEmpty=null, $recursive=null){ }


		/**
		 * Whether the record is not new and deleted
		 *
		 * @return boolean
		 */
		public function isRecord(){ }


		/**
		 * Whether the record is new and deleted
		 *
		 * @return boolean
		 */
		public function isNewRecord(){ }


		/**
		 * Whether the record is new and deleted
		 *
		 * @return boolean
		 */
		public function isDeletedRecord(){ }


		public function __debugInfo(){ }


		public function setDbService($connectionService){ }


		public function getRealAttributes(){ }


		/**
		 * Returns serialised model as array for json_encode.
		 *
		 *<code>
		 * $robot = Robots::findFirst();
		 * echo json_encode($robot);
		 *</code>
		 *
		 * @return array
		 */
		public function jsonSerialize(){ }

	}
}
