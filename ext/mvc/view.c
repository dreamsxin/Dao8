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

#include "mvc/view.h"
#include "mvc/viewinterface.h"
#include "mvc/view/engine.h"
#include "mvc/view/engineinterface.h"
#include "mvc/view/engine/php.h"
#include "mvc/view/exception.h"
#include "mvc/view/modelinterface.h"
#include "cache/backendinterface.h"
#include "di/injectable.h"
#include "debug.h"

#include <Zend/zend_closures.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/exception.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/output.h"
#include "kernel/operators.h"
#include "kernel/hash.h"
#include "kernel/concat.h"
#include "kernel/string.h"
#include "kernel/file.h"
#include "kernel/debug.h"

/**
 * Dao\Mvc\View
 *
 * Dao\Mvc\View is a class for working with the "view" portion of the model-view-controller pattern.
 * That is, it exists to help keep the view script separate from the model and controller scripts.
 * It provides a system of helpers, output filters, and variable escaping.
 *
 * <code>
 * //Setting views directory
 * $view = new Dao\Mvc\View();
 * $view->setViewsDir('app/views/');
 *
 * $view->start();
 * //Shows recent posts view (app/views/posts/recent.phtml)
 * $view->render('posts', 'recent');
 * $view->finish();
 *
 * //Printing views output
 * echo $view->getContent();
 * </code>
 */
zend_class_entry *dao_mvc_view_ce;

PHP_METHOD(Dao_Mvc_View, __construct);
PHP_METHOD(Dao_Mvc_View, setViewsDir);
PHP_METHOD(Dao_Mvc_View, getViewsDir);
PHP_METHOD(Dao_Mvc_View, setLayoutsDir);
PHP_METHOD(Dao_Mvc_View, getLayoutsDir);
PHP_METHOD(Dao_Mvc_View, setPartialsDir);
PHP_METHOD(Dao_Mvc_View, getPartialsDir);
PHP_METHOD(Dao_Mvc_View, setBasePath);
PHP_METHOD(Dao_Mvc_View, getBasePath);
PHP_METHOD(Dao_Mvc_View, getCurrentRenderLevel);
PHP_METHOD(Dao_Mvc_View, getRenderLevel);
PHP_METHOD(Dao_Mvc_View, setRenderLevel);
PHP_METHOD(Dao_Mvc_View, disableLevel);
PHP_METHOD(Dao_Mvc_View, getDisabledLevels);
PHP_METHOD(Dao_Mvc_View, setMainView);
PHP_METHOD(Dao_Mvc_View, getMainView);
PHP_METHOD(Dao_Mvc_View, setLayout);
PHP_METHOD(Dao_Mvc_View, getLayout);
PHP_METHOD(Dao_Mvc_View, setTemplateBefore);
PHP_METHOD(Dao_Mvc_View, cleanTemplateBefore);
PHP_METHOD(Dao_Mvc_View, setTemplateAfter);
PHP_METHOD(Dao_Mvc_View, cleanTemplateAfter);
PHP_METHOD(Dao_Mvc_View, setParamToView);
PHP_METHOD(Dao_Mvc_View, getParamsToView);
PHP_METHOD(Dao_Mvc_View, setVars);
PHP_METHOD(Dao_Mvc_View, setVar);
PHP_METHOD(Dao_Mvc_View, getVar);
PHP_METHOD(Dao_Mvc_View, setControllerName);
PHP_METHOD(Dao_Mvc_View, getControllerName);
PHP_METHOD(Dao_Mvc_View, setActionName);
PHP_METHOD(Dao_Mvc_View, getActionName);
PHP_METHOD(Dao_Mvc_View, setParams);
PHP_METHOD(Dao_Mvc_View, getParams);
PHP_METHOD(Dao_Mvc_View, setNamespaceName);
PHP_METHOD(Dao_Mvc_View, getNamespaceName);
PHP_METHOD(Dao_Mvc_View, start);
PHP_METHOD(Dao_Mvc_View, _loadTemplateEngines);
PHP_METHOD(Dao_Mvc_View, _engineRender);
PHP_METHOD(Dao_Mvc_View, registerEngines);
PHP_METHOD(Dao_Mvc_View, getRegisteredEngines);
PHP_METHOD(Dao_Mvc_View, getEngines);
PHP_METHOD(Dao_Mvc_View, exists);
PHP_METHOD(Dao_Mvc_View, render);
PHP_METHOD(Dao_Mvc_View, pick);
PHP_METHOD(Dao_Mvc_View, partial);
PHP_METHOD(Dao_Mvc_View, getRender);
PHP_METHOD(Dao_Mvc_View, finish);
PHP_METHOD(Dao_Mvc_View, _createCache);
PHP_METHOD(Dao_Mvc_View, isCaching);
PHP_METHOD(Dao_Mvc_View, getCache);
PHP_METHOD(Dao_Mvc_View, cache);
PHP_METHOD(Dao_Mvc_View, setContent);
PHP_METHOD(Dao_Mvc_View, getContent);
PHP_METHOD(Dao_Mvc_View, startSection);
PHP_METHOD(Dao_Mvc_View, stopSection);
PHP_METHOD(Dao_Mvc_View, section);
PHP_METHOD(Dao_Mvc_View, getActiveRenderPath);
PHP_METHOD(Dao_Mvc_View, disable);
PHP_METHOD(Dao_Mvc_View, enable);
PHP_METHOD(Dao_Mvc_View, isDisabled);
PHP_METHOD(Dao_Mvc_View, enableNamespaceView);
PHP_METHOD(Dao_Mvc_View, disableNamespaceView);
PHP_METHOD(Dao_Mvc_View, enableMultiNamespaceView);
PHP_METHOD(Dao_Mvc_View, disableMultiNamespaceView);
PHP_METHOD(Dao_Mvc_View, enableLowerCase);
PHP_METHOD(Dao_Mvc_View, disableLowerCase);
PHP_METHOD(Dao_Mvc_View, setConverter);
PHP_METHOD(Dao_Mvc_View, getConverter);
PHP_METHOD(Dao_Mvc_View, reset);
PHP_METHOD(Dao_Mvc_View, __set);
PHP_METHOD(Dao_Mvc_View, __get);
PHP_METHOD(Dao_Mvc_View, __isset);
PHP_METHOD(Dao_Mvc_View, setup);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_view___construct, 0, 0, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_mvc_view_setup, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, options, IS_ARRAY, 1)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_mvc_view_method_entry[] = {
	PHP_ME(Dao_Mvc_View, __construct, arginfo_dao_mvc_view___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Mvc_View, setViewsDir, arginfo_dao_mvc_viewinterface_setviewsdir, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getViewsDir, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setLayoutsDir, arginfo_dao_mvc_viewinterface_setlayoutsdir, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getLayoutsDir, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setPartialsDir, arginfo_dao_mvc_viewinterface_setpartialsdir, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getPartialsDir, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setBasePath, arginfo_dao_mvc_viewinterface_setbasepath, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getBasePath, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getCurrentRenderLevel, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getRenderLevel, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setRenderLevel, arginfo_dao_mvc_viewinterface_setrenderlevel, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, disableLevel, arginfo_dao_mvc_viewinterface_disablelevel, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getDisabledLevels, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setMainView, arginfo_dao_mvc_viewinterface_setmainview, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getMainView, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setLayout, arginfo_dao_mvc_viewinterface_setlayout, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getLayout, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setTemplateBefore, arginfo_dao_mvc_viewinterface_settemplatebefore, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, cleanTemplateBefore, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setTemplateAfter, arginfo_dao_mvc_viewinterface_settemplateafter, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, cleanTemplateAfter, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setParamToView, arginfo_dao_mvc_viewinterface_setparamtoview, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getParamsToView, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setVars, arginfo_dao_mvc_viewinterface_setvars, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setVar, arginfo_dao_mvc_viewinterface_setvar, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getVar, arginfo_dao_mvc_viewinterface_getvar, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setControllerName, arginfo_dao_mvc_viewinterface_setcontrollername, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getControllerName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setActionName, arginfo_dao_mvc_viewinterface_setactionname, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getActionName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setParams, arginfo_dao_mvc_viewinterface_setparams, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getParams, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setNamespaceName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getNamespaceName, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, start, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, _loadTemplateEngines, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Mvc_View, _engineRender, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Mvc_View, registerEngines, arginfo_dao_mvc_viewinterface_registerengines, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getRegisteredEngines, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getEngines, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, exists, arginfo_dao_mvc_viewinterface_exists, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, render, arginfo_dao_mvc_viewinterface_render, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, pick, arginfo_dao_mvc_viewinterface_pick, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, partial, arginfo_dao_mvc_viewinterface_partial, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getRender, arginfo_dao_mvc_viewinterface_getrender, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, finish, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, _createCache, arginfo_empty, ZEND_ACC_PROTECTED)
	PHP_ME(Dao_Mvc_View, isCaching, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getCache, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, cache, arginfo_dao_mvc_viewinterface_cache, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setContent, arginfo_dao_mvc_viewinterface_setcontent, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getContent, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, startSection, arginfo_dao_mvc_viewinterface_startsection, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, stopSection, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, section, arginfo_dao_mvc_viewinterface_section, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getActiveRenderPath, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, disable, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, enable, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, isDisabled, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, enableNamespaceView, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, disableNamespaceView, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, enableMultiNamespaceView, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, disableMultiNamespaceView, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, enableLowerCase, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, disableLowerCase, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setConverter, arginfo_dao_mvc_viewinterface_setconverter, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, getConverter, arginfo_dao_mvc_viewinterface_getconverter, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, reset, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, __set, arginfo___set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, __get, arginfo___get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, __isset, arginfo___isset, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Mvc_View, setup, arginfo_dao_mvc_view_setup, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_MALIAS(Dao_Mvc_View, insert, partial, arginfo_dao_mvc_viewinterface_partial, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Mvc\View initializer
 */
DAO_INIT_CLASS(Dao_Mvc_View){

	DAO_REGISTER_CLASS_EX(Dao\\Mvc, View, mvc_view, dao_di_injectable_ce, dao_mvc_view_method_entry, 0);

	zend_declare_property_null(dao_mvc_view_ce, SL("_options"), ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_mvc_view_ce, SL("_basePath"), "", ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_mvc_view_ce, SL("_content"), "", ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_sections"), ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_mvc_view_ce, SL("_renderLevel"), 6, ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_mvc_view_ce, SL("_currentRenderLevel"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_disabledLevels"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_viewParams"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_layout"), ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_mvc_view_ce, SL("_layoutsDir"), "", ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_mvc_view_ce, SL("_partialsDir"), "", ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_mvc_view_ce, SL("_enableLayoutsAbsolutePath"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_mvc_view_ce, SL("_enablePartialsAbsolutePath"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_viewsDir"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_mvc_view_ce, SL("_enableNamespaceView"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_mvc_view_ce, SL("_enableMultiNamespaceView"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_templatesBefore"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_templatesAfter"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_mvc_view_ce, SL("_engines"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_registeredEngines"), ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_mvc_view_ce, SL("_mainView"), "index", ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_controllerName"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_namespaceName"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_actionName"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_params"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_pickView"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_cache"), ZEND_ACC_PROTECTED);
	zend_declare_property_long(dao_mvc_view_ce, SL("_cacheLevel"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_mvc_view_ce, SL("_cacheMode"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_activeRenderPath"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_mvc_view_ce, SL("_disabled"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_mvc_view_ce, SL("_lowerCase"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_mvc_view_ce, SL("_converters"), ZEND_ACC_PROTECTED);

	/**
	 * Render level
	 */
	zend_declare_class_constant_long(dao_mvc_view_ce, SL("LEVEL_MAIN"), DAO_VIEW_LEVEL_MAIN);
	zend_declare_class_constant_long(dao_mvc_view_ce, SL("LEVEL_MAIN_LAYOUT"), DAO_VIEW_LEVEL_MAIN);
	zend_declare_class_constant_long(dao_mvc_view_ce, SL("LEVEL_AFTER_TEMPLATE"), DAO_VIEW_LEVEL_AFTER_TEMPLATE);
	zend_declare_class_constant_long(dao_mvc_view_ce, SL("LEVEL_NAMESPACE"), DAO_VIEW_LEVEL_NAMESPACE);
	zend_declare_class_constant_long(dao_mvc_view_ce, SL("LEVEL_CONTROLLER"), DAO_VIEW_LEVEL_CONTROLLER);
	zend_declare_class_constant_long(dao_mvc_view_ce, SL("LEVEL_LAYOUT"), DAO_VIEW_LEVEL_CONTROLLER);
	zend_declare_class_constant_long(dao_mvc_view_ce, SL("LEVEL_BEFORE_TEMPLATE"), DAO_VIEW_LEVEL_BEFORE_TEMPLATE);
	zend_declare_class_constant_long(dao_mvc_view_ce, SL("LEVEL_ACTION"), DAO_VIEW_LEVEL_ACTION);
	zend_declare_class_constant_long(dao_mvc_view_ce, SL("LEVEL_ACTION_VIEW"), DAO_VIEW_LEVEL_ACTION);
	zend_declare_class_constant_long(dao_mvc_view_ce, SL("LEVEL_NO_RENDER"), DAO_VIEW_LEVEL_NO_RENDER);

	zend_declare_class_constant_bool(dao_mvc_view_ce, SL("CACHE_MODE_NONE"), 0);
	zend_declare_class_constant_bool(dao_mvc_view_ce, SL("CACHE_MODE_INVERSE"), 1);

	zend_class_implements(dao_mvc_view_ce, 1, dao_mvc_viewinterface_ce);

	return SUCCESS;
}

/**
 * Dao\Mvc\View constructor
 *
 * @param array $options
 */
PHP_METHOD(Dao_Mvc_View, __construct){

	zval *options = NULL;

	dao_fetch_params(0, 0, 1, &options);

	if (options && Z_TYPE_P(options) == IS_ARRAY) {
		dao_update_property(getThis(), SL("_options"), options);
	}

	dao_update_property_empty_array(getThis(), SL("_sections"));
	dao_update_property_empty_array(getThis(), SL("_converters"));
}

/**
 * Sets views directory. Depending of your platform, always add a trailing slash or backslash
 *
 * @param string $viewsDir
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setViewsDir){

	zval *views_dir, path = {};

	dao_fetch_params(0, 1, 0, &views_dir);

	dao_add_trailing_slash(&path, views_dir);
	dao_update_property(getThis(), SL("_viewsDir"), &path);
	zval_ptr_dtor(&path);

	RETURN_THIS();
}

/**
 * Gets views directory
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View, getViewsDir){


	RETURN_MEMBER(getThis(), "_viewsDir");
}

/**
 * Sets the layouts sub-directory. Must be a directory under the views directory. Depending of your platform, always add a trailing slash or backslash
 *
 *<code>
 * $view->setLayoutsDir('../common/layouts/');
 *</code>
 *
 * @param string $layoutsDir
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setLayoutsDir){

	zval *layouts_dir, *absolute_path = NULL, path = {};
	int absolute = 0;

	dao_fetch_params(0, 1, 1, &layouts_dir, &absolute_path);
	dao_add_trailing_slash(&path, layouts_dir);
	dao_update_property(getThis(), SL("_layoutsDir"), &path);
	zval_ptr_dtor(&path);
	absolute = absolute_path ? zend_is_true(absolute_path) : 0;
	dao_update_property_bool(getThis(), SL("_enableLayoutsAbsolutePath"), absolute);
	RETURN_THIS();
}

/**
 * Gets the current layouts sub-directory
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View, getLayoutsDir){


	RETURN_MEMBER(getThis(), "_layoutsDir");
}

/**
 * Sets a partials sub-directory. Must be a directory under the views directory. Depending of your platform, always add a trailing slash or backslash
 *
 *<code>
 * $view->setPartialsDir('../common/partials/');
 *</code>
 *
 * @param string $partialsDir
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setPartialsDir){

	zval *partials_dir, *absolute_path = NULL, path = {};
	int absolute = 0;

	dao_fetch_params(0, 1, 1, &partials_dir, &absolute_path);
	dao_add_trailing_slash(&path, partials_dir);
	absolute = absolute_path ? zend_is_true(absolute_path) : 0;
	dao_update_property(getThis(), SL("_partialsDir"), &path);
	zval_ptr_dtor(&path);
	dao_update_property_bool(getThis(), SL("_enablePartialsAbsolutePath"), absolute);
	RETURN_THIS();
}

/**
 * Gets the current partials sub-directory
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View, getPartialsDir){


	RETURN_MEMBER(getThis(), "_partialsDir");
}

/**
 * Sets base path. Depending of your platform, always add a trailing slash or backslash
 *
 * <code>
 * 	$view->setBasePath(__DIR__ . '/');
 * </code>
 *
 * @param string|array $basePath
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setBasePath){

	zval *base_path, paths, *path;

	dao_fetch_params(0, 1, 0, &base_path);

	if (Z_TYPE_P(base_path) == IS_ARRAY) {
		array_init(&paths);

		ZEND_HASH_FOREACH_VAL(Z_ARRVAL_P(base_path), path) {
			zval tmp = {};
			dao_add_trailing_slash(&tmp, path);
			zval_ptr_dtor(&tmp);
			dao_array_append(&paths, path, PH_COPY);
		} ZEND_HASH_FOREACH_END();

		dao_update_property(getThis(), SL("_basePath"), &paths);
		zval_ptr_dtor(&paths);
	} else {
		zval tmp = {};
		dao_add_trailing_slash(&tmp, base_path);
		dao_update_property(getThis(), SL("_basePath"), &tmp);
		zval_ptr_dtor(&tmp);
	}

	RETURN_THIS();
}

/**
 * Gets base path
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View, getBasePath){

	RETURN_MEMBER(getThis(), "_basePath");
}

/**
 * Returns the render level for the view
 *
 * @return int
 */
PHP_METHOD(Dao_Mvc_View, getCurrentRenderLevel) {

	RETURN_MEMBER(getThis(), "_currentRenderLevel");
}

/**
 * Returns the render level for the view
 *
 * @return int
 */
PHP_METHOD(Dao_Mvc_View, getRenderLevel) {

	RETURN_MEMBER(getThis(), "_renderLevel");
}

/**
 * Sets the render level for the view
 *
 * <code>
 * 	//Render the view related to the controller only
 * 	$this->view->setRenderLevel(View::LEVEL_LAYOUT);
 * </code>
 *
 * @param string $level
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setRenderLevel){

	zval *level;

	dao_fetch_params(0, 1, 0, &level);

	dao_update_property(getThis(), SL("_renderLevel"), level);
	RETURN_THIS();
}

/**
 * Disables a specific level of rendering
 *
 *<code>
 * //Render all levels except ACTION level
 * $this->view->disableLevel(View::LEVEL_ACTION_VIEW);
 *</code>
 *
 * @param int|array $level
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, disableLevel){

	zval *level;

	dao_fetch_params(0, 1, 0, &level);

	if (Z_TYPE_P(level) == IS_ARRAY) {
		dao_update_property(getThis(), SL("_disabledLevels"), level);
	} else {
		dao_update_property_array(getThis(), SL("_disabledLevels"), level, &DAO_GLOBAL(z_true));
	}

	RETURN_THIS();
}

/**
 * Returns an array with disabled render levels
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_View, getDisabledLevels) {

	RETURN_MEMBER(getThis(), "_disabledLevels");
}

/**
 * Sets default view name. Must be a file without extension in the views directory
 *
 * <code>
 * 	//Renders as main view views-dir/base.phtml
 * 	$this->view->setMainView('base');
 * </code>
 *
 * @param string $viewPath
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setMainView){

	zval *view_path;

	dao_fetch_params(0, 1, 0, &view_path);

	dao_update_property(getThis(), SL("_mainView"), view_path);
	RETURN_THIS();
}

/**
 * Returns the name of the main view
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View, getMainView){


	RETURN_MEMBER(getThis(), "_mainView");
}

/**
 * Change the layout to be used instead of using the name of the latest controller name
 *
 * <code>
 * 	$this->view->setLayout('main');
 * </code>
 *
 * @param string $layout
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setLayout){

	zval *layout;

	dao_fetch_params(0, 1, 0, &layout);

	dao_update_property(getThis(), SL("_layout"), layout);
	RETURN_THIS();
}

/**
 * Returns the name of the main view
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View, getLayout){


	RETURN_MEMBER(getThis(), "_layout");
}

/**
 * Appends template before controller layout
 *
 * @param string|array $templateBefore
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setTemplateBefore){

	zval *template_before, array_template = {};

	dao_fetch_params(0, 1, 0, &template_before);

	if (Z_TYPE_P(template_before) != IS_ARRAY) {
		array_init_size(&array_template, 1);
		dao_array_append(&array_template, template_before, PH_COPY);
		dao_update_property(getThis(), SL("_templatesBefore"), &array_template);
		zval_ptr_dtor(&array_template);
	} else {
		dao_update_property(getThis(), SL("_templatesBefore"), template_before);
	}

	RETURN_THIS();
}

/**
 * Resets any template before layouts
 *
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, cleanTemplateBefore){


	dao_update_property_null(getThis(), SL("_templatesBefore"));
	RETURN_THIS();
}

/**
 * Appends template after controller layout
 *
 * @param string|array $templateAfter
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setTemplateAfter){

	zval *template_after, array_template = {};

	dao_fetch_params(0, 1, 0, &template_after);

	if (Z_TYPE_P(template_after) != IS_ARRAY) {
		array_init_size(&array_template, 1);
		dao_array_append(&array_template, template_after, PH_COPY);
		dao_update_property(getThis(), SL("_templatesAfter"), &array_template);
		zval_ptr_dtor(&array_template);
	} else {
		dao_update_property(getThis(), SL("_templatesAfter"), template_after);
	}

	RETURN_THIS();
}

/**
 * Resets any template after layouts
 *
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, cleanTemplateAfter){


	dao_update_property_null(getThis(), SL("_templatesAfter"));
	RETURN_THIS();
}

/**
 * Adds parameters to views (alias of setVar)
 *
 *<code>
 *	$this->view->setParamToView('products', $products);
 *</code>
 *
 * @param string $key
 * @param mixed $value
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setParamToView){

	zval *key, *value;

	dao_fetch_params(0, 2, 0, &key, &value);

	dao_update_property_array(getThis(), SL("_viewParams"), key, value);
	RETURN_THIS();
}

/**
 * Returns parameters to views
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_View, getParamsToView){


	RETURN_MEMBER(getThis(), "_viewParams");
}

/**
 * Set all the render params
 *
 *<code>
 *	$this->view->setVars(array('products' => $products));
 *</code>
 *
 * @param array $params
 * @param boolean $merge
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setVars){

	zval *params, *merge = NULL, view_params = {};

	dao_fetch_params(0, 1, 1, &params, &merge);
	DAO_SEPARATE_PARAM(params);

	if (!merge) {
		merge = &DAO_GLOBAL(z_true);
	}

	if (Z_TYPE_P(params) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_view_exception_ce, "The render parameters must be an array");
		return;
	}
	if (zend_is_true(merge)) {
		dao_read_property(&view_params, getThis(), SL("_viewParams"), PH_NOISY|PH_READONLY);
		if (Z_TYPE(view_params) == IS_ARRAY) {
			dao_array_replace(&view_params, params);
		} else {
			dao_update_property(getThis(), SL("_viewParams"), params);
		}
	} else {
		dao_update_property(getThis(), SL("_viewParams"), params);
	}

	RETURN_THIS();
}

/**
 * Set a single view parameter
 *
 *<code>
 *	$this->view->setVar('products', $products);
 *</code>
 *
 * @param string $key
 * @param mixed $value
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setVar){

	zval *key, *value;

	dao_fetch_params(0, 2, 0, &key, &value);

	dao_update_property_array(getThis(), SL("_viewParams"), key, value);
	RETURN_THIS();
}

/**
 * Returns a parameter previously set in the view
 *
 * @param string $key
 * @return mixed
 */
PHP_METHOD(Dao_Mvc_View, getVar){

	zval *key, params = {}, value = {};

	dao_fetch_params(0, 1, 0, &key);

	dao_read_property(&params, getThis(), SL("_viewParams"), PH_NOISY|PH_READONLY);
	if (dao_array_isset_fetch(&value, &params, key, PH_READONLY)) {
		RETURN_CTOR(&value);
	}

	RETURN_NULL();
}

/**
 * Sets the controller name to be view
 *
 * @param string $controllerName
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setControllerName){

	zval *controller_name;

	dao_fetch_params(0, 1, 0, &controller_name);

	dao_update_property(getThis(), SL("_controllerName"), controller_name);
	RETURN_THIS();
}

/**
 * Gets the name of the controller rendered
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View, getControllerName){


	RETURN_MEMBER(getThis(), "_controllerName");
}

/**
 * Sets the action name to be view
 *
 * @param string $actionName
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setActionName){

	zval *action_name;

	dao_fetch_params(0, 1, 0, &action_name);

	dao_update_property(getThis(), SL("_actionName"), action_name);
	RETURN_THIS();
}

/**
 * Gets the name of the action rendered
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View, getActionName){


	RETURN_MEMBER(getThis(), "_actionName");
}

/**
 * Sets the extra parameters to be view
 *
 * @param array $params
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setParams){

	zval *params;

	dao_fetch_params(0, 1, 0, &params);

	dao_update_property(getThis(), SL("_params"), params);
	RETURN_THIS();
}

/**
 * Gets extra parameters of the action rendered
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_View, getParams){


	RETURN_MEMBER(getThis(), "_params");
}

PHP_METHOD(Dao_Mvc_View, setNamespaceName){

	zval *namespace_name;

	dao_fetch_params(0, 1, 0, &namespace_name);

	dao_update_property(getThis(), SL("_namespaceName"), namespace_name);
	RETURN_THIS();
}

PHP_METHOD(Dao_Mvc_View, getNamespaceName){


	RETURN_MEMBER(getThis(), "_namespaceName");
}

/**
 * Starts rendering process enabling the output buffering
 *
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, start){

	dao_update_property_null(getThis(), SL("_content"));
	dao_ob_start();
	RETURN_THIS();
}

/**
 * Loads registered template engines, if none is registered it will use Dao\Mvc\View\Engine\Php
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_View, _loadTemplateEngines){

	zval dependency_injector = {}, registered_engines = {}, php_engine = {}, arguments = {}, *engine_service;
	zend_string *str_key;

	DAO_MM_INIT();
	dao_read_property(return_value, getThis(), SL("_engines"), PH_COPY);

	/**
	 * If the engines aren't initialized 'engines' is false
	 */
	if (DAO_IS_FALSE(return_value)) {
		DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
		DAO_MM_ADD_ENTRY(&dependency_injector);

		array_init(return_value);

		dao_read_property(&registered_engines, getThis(), SL("_registeredEngines"), PH_NOISY|PH_READONLY);
		if (Z_TYPE(registered_engines) != IS_ARRAY) {
			/**
			 * We use Dao\Mvc\View\Engine\Php as default
			 */
			object_init_ex(&php_engine, dao_mvc_view_engine_php_ce);
			DAO_MM_ADD_ENTRY(&php_engine);
			DAO_MM_CALL_METHOD(NULL, &php_engine, "__construct", getThis(), &dependency_injector);

			dao_array_update_str(return_value, SL(".phtml"), &php_engine, PH_COPY);
		} else {
			if (Z_TYPE(dependency_injector) != IS_OBJECT) {
				DAO_MM_THROW_EXCEPTION_STR(dao_mvc_view_exception_ce, "A dependency injector container is required to obtain the application services");
				return;
			}

			array_init_size(&arguments, 2);
			DAO_MM_ADD_ENTRY(&arguments);
			dao_array_append(&arguments, getThis(), PH_COPY);
			dao_array_append(&arguments, &dependency_injector, PH_COPY);

			ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL(registered_engines), str_key, engine_service) {
				zval extension = {}, engine_object = {}, exception_message = {};
				if (str_key) {
					ZVAL_STR(&extension, str_key);
					if (Z_TYPE_P(engine_service) == IS_OBJECT) {

						/**
						 * Engine can be a closure
						 */
						if (instanceof_function(Z_OBJCE_P(engine_service), zend_ce_closure)) {
							DAO_MM_CALL_USER_FUNC_ARRAY(&engine_object, engine_service, &arguments);
						} else {
							ZVAL_COPY_VALUE(&engine_object, engine_service);
						}
					} else {
						/**
						 * Engine can be a string representing a service in the DI
						 */
						if (Z_TYPE_P(engine_service) == IS_STRING) {
							DAO_MM_CALL_METHOD(&engine_object, &dependency_injector, "getshared", engine_service, &arguments);
							DAO_MM_ADD_ENTRY(&engine_object);
							DAO_MM_VERIFY_INTERFACE(&engine_object, dao_mvc_view_engineinterface_ce);
						} else {
							DAO_CONCAT_SV(&exception_message, "Invalid template engine registration for extension: ", &extension);
							DAO_MM_ADD_ENTRY(&exception_message);
							DAO_MM_THROW_EXCEPTION_ZVAL(dao_mvc_view_exception_ce, &exception_message);
							return;
						}
					}
					dao_array_update(return_value, &extension, &engine_object, PH_COPY);
				}
			} ZEND_HASH_FOREACH_END();
		}
		dao_update_property(getThis(), SL("_engines"), return_value);
	}
	RETURN_MM();
}

/**
 * Checks whether view exists on registered extensions and render it
 *
 * @param array $engines
 * @param string $viewPath
 * @param boolean $silence
 * @param boolean $mustClean
 */
PHP_METHOD(Dao_Mvc_View, _engineRender){

	zval *engines, *view_path, *silence, *must_clean, *absolute_path = NULL, debug_message = {}, render_level = {}, cache_level = {};
	zval cache_mode = {}, cache = {}, not_exists = {}, views_dir_paths = {}, base_path = {}, views_dir = {}, *path;
	zval key = {}, lifetime = {}, view_options = {}, cache_options = {}, cached_view = {};
	zval view_params = {}, *engine, event_name = {}, status = {}, exception_message = {};
	zend_string *str_key;

	dao_fetch_params(1, 4, 1, &engines, &view_path, &silence, &must_clean, &absolute_path);

	if (absolute_path == NULL) {
		absolute_path = &DAO_GLOBAL(z_false);
	}

	/**
	 * Start the cache if there is a cache level enabled
	 */
	dao_read_property(&cache_level, getThis(), SL("_cacheLevel"), PH_NOISY|PH_READONLY);

	if (zend_is_true(&cache_level)) {
		dao_read_property(&render_level, getThis(), SL("_currentRenderLevel"), PH_NOISY|PH_READONLY);
		dao_read_property(&cache_mode, getThis(), SL("_cacheMode"), PH_NOISY|PH_READONLY);

		if (DAO_IS_TRUE(&cache_mode)) {
			if (DAO_LE(&render_level, &cache_level)) {
				DAO_MM_CALL_METHOD(&cache, getThis(), "getcache");
				DAO_MM_ADD_ENTRY(&cache);
			}
		} else {
			if (DAO_GE(&render_level, &cache_level)) {
				DAO_MM_CALL_METHOD(&cache, getThis(), "getcache");
				DAO_MM_ADD_ENTRY(&cache);
			}
		}
	}

	ZVAL_TRUE(&not_exists);
	array_init(&views_dir_paths);
	DAO_MM_ADD_ENTRY(&views_dir_paths);
	if (zend_is_true(absolute_path)) {
		dao_array_append(&views_dir_paths, view_path, PH_COPY);
	} else {
		dao_read_property(&base_path, getThis(), SL("_basePath"), PH_NOISY|PH_READONLY);
		dao_read_property(&views_dir, getThis(), SL("_viewsDir"), PH_NOISY|PH_READONLY);

		if (Z_TYPE(base_path) == IS_ARRAY) {
			ZEND_HASH_FOREACH_VAL(Z_ARRVAL(base_path), path) {
				zval *dir;
				if (Z_TYPE(views_dir) == IS_ARRAY) {
					ZEND_HASH_FOREACH_VAL(Z_ARRVAL(views_dir), dir) {
						zval views_dir_path = {};
						DAO_CONCAT_VVV(&views_dir_path, path, dir, view_path);
						dao_array_append(&views_dir_paths, &views_dir_path, 0);
					} ZEND_HASH_FOREACH_END();
				} else {
					zval views_dir_path = {};
					DAO_CONCAT_VVV(&views_dir_path, path, &views_dir, view_path);
					dao_array_append(&views_dir_paths, &views_dir_path, 0);
				}
			} ZEND_HASH_FOREACH_END();
		} else {
			if (Z_TYPE(views_dir) == IS_ARRAY) {
				ZEND_HASH_FOREACH_VAL(Z_ARRVAL(views_dir), path) {
					zval views_dir_path = {};
					DAO_CONCAT_VVV(&views_dir_path, &base_path, path, view_path);
					dao_array_append(&views_dir_paths, &views_dir_path, 0);
				} ZEND_HASH_FOREACH_END();
			} else {
				zval views_dir_path = {};
				DAO_CONCAT_VVV(&views_dir_path, &base_path, &views_dir, view_path);
				dao_array_append(&views_dir_paths, &views_dir_path, 0);
			}
		}
	}

	if (Z_TYPE(cache) == IS_OBJECT) {
		/**
		 * Check if the cache is started, the first time a cache is started we start the
		 * cache
		 */
		dao_read_property(&view_options, getThis(), SL("_options"), PH_READONLY);

		/**
		 * Check if the user has defined a different options to the default
		 */
		ZVAL_NULL(&lifetime);
		if (Z_TYPE(view_options) == IS_ARRAY) {
			if (dao_array_isset_fetch_str(&cache_options, &view_options, SL("cache"), PH_READONLY)) {
				if (Z_TYPE(cache_options) == IS_ARRAY) {
					if (dao_array_isset_str(&cache_options, SL("key"))) {
						dao_array_fetch_str(&key, &cache_options, SL("key"), PH_NOISY|PH_READONLY);
					}
					if (dao_array_isset_str(&cache_options, SL("lifetime"))) {
						dao_array_fetch_str(&lifetime, &cache_options, SL("lifetime"), PH_NOISY|PH_READONLY);
					}
				}
			}
		}

		/**
		 * If a cache key is not set we create one using a md5
		 */
		if (Z_TYPE(key) <= IS_NULL) {
			dao_md5(&key, view_path);
			DAO_MM_ADD_ENTRY(&key);
		}

		/**
		 * We start the cache using the key set
		 */
		if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
			DAO_CONCAT_SV(&debug_message, "--Get view cache: ", &key);
			DAO_DEBUG_LOG(&debug_message);
			zval_ptr_dtor(&debug_message);
		}

		DAO_MM_CALL_METHOD(&cached_view, &cache, "start", &key, &lifetime, &DAO_GLOBAL(z_true));
		DAO_MM_ADD_ENTRY(&cached_view);
		if (Z_TYPE(cached_view) != IS_NULL) {
			dao_update_property(getThis(), SL("_content"), &cached_view);
			RETURN_MM_NULL();
		}
	}

	dao_read_property(&view_params, getThis(), SL("_viewParams"), PH_NOISY|PH_READONLY);

	if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
		DAO_CONCAT_SV(&debug_message, "Render View: ", view_path);
		DAO_DEBUG_LOG(&debug_message);
		zval_ptr_dtor(&debug_message);
		ZVAL_STRING(&debug_message, "--vars: ");
		DAO_DEBUG_LOG(&debug_message);
		zval_ptr_dtor(&debug_message);
		DAO_DEBUG_LOG(&view_params);
	}

	/**
	 * Views are rendered in each engine
	 */
	ZEND_HASH_FOREACH_STR_KEY_VAL(Z_ARRVAL_P(engines), str_key, engine) {
		zval extension = {};
		if (!str_key) {
			continue;
		}
		ZVAL_STR(&extension, str_key);
		ZEND_HASH_FOREACH_VAL(Z_ARRVAL(views_dir_paths), path) {
			zval view_engine_path = {};
			DAO_CONCAT_VV(&view_engine_path, path, &extension);
			DAO_MM_ADD_ENTRY(&view_engine_path);
			if (dao_file_exists(&view_engine_path) != SUCCESS) {
				if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
					DAO_CONCAT_SV(&debug_message, "--Not Found: ", &view_engine_path);
					DAO_DEBUG_LOG(&debug_message);
					zval_ptr_dtor(&debug_message);
				}
				continue;
			}

			dao_update_property(getThis(), SL("_activeRenderPath"), &view_engine_path);
			if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
				DAO_CONCAT_SV(&debug_message, "--Found: ", &view_engine_path);
				DAO_DEBUG_LOG(&debug_message);
				zval_ptr_dtor(&debug_message);
			}

			/**
			 * Call beforeRenderView if there is a events manager available
			 */
			DAO_MM_ZVAL_STRING(&event_name, "view:beforeRenderView");
			DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name, &view_engine_path);

			if (DAO_IS_FALSE(&status)) {
				continue;
			}
			zval_ptr_dtor(&status);

			DAO_MM_CALL_METHOD(NULL, engine, "render", &view_engine_path, &view_params, must_clean);

			/**
			 * Call afterRenderView if there is a events manager available
			 */
			ZVAL_FALSE(&not_exists);

			DAO_MM_ZVAL_STRING(&event_name, "view:afterRenderView");
			DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name);
			break;
		} ZEND_HASH_FOREACH_END();

		if (!zend_is_true(&not_exists)) {
			break;
		}
	} ZEND_HASH_FOREACH_END();

	if (DAO_IS_TRUE(&not_exists)) {
		if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
			ZVAL_STRING(&debug_message, "--Not Found View");
			DAO_DEBUG_LOG(&debug_message);
			zval_ptr_dtor(&debug_message);
		}

		if (dao_ob_get_level() >= 1) {
			zval contents = {};
			dao_ob_get_contents(&contents);
			dao_ob_clean();
			if (Z_TYPE(contents) > IS_NULL) {
				DAO_MM_ADD_ENTRY(&contents);
				DAO_MM_CALL_METHOD(NULL, getThis(), "setcontent", &contents, &DAO_GLOBAL(z_true));
			}
		}

		/**
		 * Notify about not found views
		 */
		DAO_MM_ZVAL_STRING(&event_name, "view:notFoundView");
		DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name);

		if (!zend_is_true(silence)) {
			DAO_CONCAT_SVS(&exception_message, "View '", view_path, "' was not found in the views directory");
			DAO_MM_ADD_ENTRY(&exception_message);
			DAO_MM_THROW_EXCEPTION_ZVAL(dao_mvc_view_exception_ce, &exception_message);
			return;
		}
	}

	/**
	 * Store the data in the cache
	 */
	if (Z_TYPE(cache) == IS_OBJECT) {
		zval contents = {};
		DAO_MM_CALL_METHOD(&contents, getThis(), "getcontent");
		DAO_MM_ADD_ENTRY(&contents);
		DAO_MM_CALL_METHOD(NULL, &cache, "save", &DAO_GLOBAL(z_null), &contents);

		if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
			ZVAL_STRING(&debug_message, "--Save view cache");
			DAO_DEBUG_LOG(&debug_message);
			zval_ptr_dtor(&debug_message);
		}
	}
	RETURN_MM();
}

/**
 * Register templating engines
 *
 *<code>
 *$this->view->registerEngines(array(
 *  ".phtml" => "Dao\Mvc\View\Engine\Php",
 *  ".volt" => "Dao\Mvc\View\Engine\Volt",
 *  ".mhtml" => "MyCustomEngine"
 *));
 *</code>
 *
 * @param array $engines
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, registerEngines){

	zval *engines;

	dao_fetch_params(0, 1, 0, &engines);

	if (Z_TYPE_P(engines) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_view_exception_ce, "Engines to register must be an array");
		return;
	}
	dao_update_property(getThis(), SL("_registeredEngines"), engines);

	RETURN_THIS();
}

/**
 * Returns the registered templating engines
 *
 * @brief array Dao\Mvc\View::getRegisteredEngines()
 */
PHP_METHOD(Dao_Mvc_View, getRegisteredEngines) {

	RETURN_MEMBER(getThis(), "_registeredEngines")
}

/**
 * Returns the registered templating engines, if none is registered it will use Dao\Mvc\View\Engine\Php
 *
 * @return array
 */
PHP_METHOD(Dao_Mvc_View, getEngines) {

	DAO_RETURN_CALL_METHOD(getThis(), "_loadtemplateengines");
}

/**
 * Checks whether a view file exists
 *
 * @param string $view
 * @param boolean $absolutePath
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_View, exists) {

	zval *view, *absolute_path = NULL, base_dir = {}, view_dir = {}, engines = {}, path = {};
	zend_string *str_key;
	int exists = 0;

	dao_fetch_params(1, 1, 1, &view, &absolute_path);
	DAO_ENSURE_IS_STRING(view);

	if (absolute_path && zend_is_true(absolute_path)) {
		ZVAL_COPY_VALUE(&path, view);
	} else {
		dao_read_property(&base_dir, getThis(), SL("_basePath"), PH_NOISY|PH_READONLY);
		dao_read_property(&view_dir, getThis(), SL("_viewsDir"), PH_NOISY|PH_READONLY);
		DAO_CONCAT_VVV(&path, &base_dir, &view_dir, view);
		DAO_MM_ADD_ENTRY(&path);
	}

	DAO_MM_CALL_METHOD(&engines, getThis(), "getengines");
	DAO_MM_ADD_ENTRY(&engines);
	ZEND_HASH_FOREACH_STR_KEY(Z_ARRVAL(engines), str_key) {
		zval ext = {}, filepath = {};
		if (str_key) {
			ZVAL_STR(&ext, str_key);
			DAO_CONCAT_VV(&filepath, &path, &ext);
			DAO_MM_ADD_ENTRY(&filepath);
			if (SUCCESS == dao_file_exists(&filepath)) {
				exists = 1;
				break;
			}
		}
	} ZEND_HASH_FOREACH_END();

	RETURN_MM_BOOL(exists);
}

/**
 * Executes render process from dispatching data
 *
 *<code>
 * //Shows recent posts view (app/views/posts/recent.phtml)
 * $view->start()->render('posts', 'recent')->finish();
 *</code>
 *
 * @param string $controllerName
 * @param string $actionName
 * @param array $params
 * @param string $namespace_name
 * @param Dao\Mvc\View\ModelInterface $viewModel
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, render){

	zval *controller, *action, *params = NULL, *namespace = NULL, *view_model = NULL, ds = {}, namespace_separator = {}, disabled = {};
	zval controller_name = {}, action_name = {}, namespace_name = {};
	zval contents = {}, converter_key = {}, converter = {}, parameters = {}, lower_case = {}, lower_controller_name = {}, lower_action_name = {};
	zval layouts_dir = {}, enable_namespace_view = {}, ds_lower_namespace_name = {}, layout_namespace = {}, debug_message = {};
	zval layout = {}, layout_name = {}, engines = {}, pick_view = {}, render_view = {}, pick_view_action = {}, event_name = {}, status = {}, silence = {}, disabled_levels = {};
	zval render_level = {}, enable_layouts_absolute_path = {}, templates_before = {}, *tpl, view_tpl_path = {}, templates_after = {}, main_view = {};
	char slash[2] = {DEFAULT_SLASH, 0};

	dao_fetch_params(1, 2, 3, &controller, &action, &params, &namespace, &view_model);

	if (!params) {
		params = &DAO_GLOBAL(z_null);
	}

	DAO_MM_ZVAL_COPY(&controller_name, controller);
	if (action) {
		DAO_MM_ZVAL_COPY(&action_name, action);
	} else {
		ZVAL_NULL(&action_name);
	}

	if (namespace) {
		DAO_MM_ZVAL_COPY(&namespace_name, namespace);
	} else {
		ZVAL_NULL(&namespace_name);
	}

	DAO_MM_ZVAL_STRING(&ds, slash);
	DAO_MM_ZVAL_STRING(&namespace_separator, "\\");

	dao_update_property(getThis(), SL("_currentRenderLevel"), &DAO_GLOBAL(z_zero));

	/**
	 * If the view is disabled we simply update the buffer from any output produced in
	 * the controller
	 */
	 
	if (dao_ob_get_level() >= 1) {
		dao_read_property(&disabled, getThis(), SL("_disabled"), PH_NOISY|PH_READONLY);
		if (DAO_IS_NOT_FALSE(&disabled)) {
			dao_ob_get_contents(&contents);
			dao_ob_clean();
			DAO_MM_ADD_ENTRY(&contents);
			if (Z_TYPE(contents) == IS_STRING) {
				DAO_MM_CALL_METHOD(NULL, getThis(), "setcontent", &contents, &DAO_GLOBAL(z_true));
			}
			RETURN_MM_FALSE;
		}
	}

	dao_update_property(getThis(), SL("_controllerName"), &controller_name);
	dao_update_property(getThis(), SL("_actionName"), &action_name);
	dao_update_property(getThis(), SL("_params"), params);
	dao_update_property(getThis(), SL("_namespaceName"), &namespace_name);

	DAO_MM_ZVAL_STRING(&converter_key, "controller");
	DAO_MM_CALL_SELF(&converter, "getconverter", &converter_key);
	DAO_MM_ADD_ENTRY(&converter);

	if (dao_is_callable(&converter)) {
		zval tmp = {};
		array_init_size(&parameters, 1);
		DAO_MM_ADD_ENTRY(&parameters);
		dao_array_append(&parameters, &controller_name, PH_COPY);

		DAO_MM_CALL_USER_FUNC_ARRAY(&tmp, &converter, &parameters);
		DAO_MM_ADD_ENTRY(&tmp);
		ZVAL_COPY_VALUE(&controller_name, &tmp);
	}

	DAO_MM_ZVAL_STRING(&converter_key, "action");
	DAO_MM_CALL_SELF(&converter, "getconverter", &converter_key);
	DAO_MM_ADD_ENTRY(&converter);

	if (dao_is_callable(&converter)) {
		zval tmp = {};

		array_init_size(&parameters, 1);
		DAO_MM_ADD_ENTRY(&parameters);
		dao_array_append(&parameters, &action_name, PH_COPY);
		DAO_MM_CALL_USER_FUNC_ARRAY(&tmp, &converter, &parameters);
		DAO_MM_ADD_ENTRY(&tmp);
		ZVAL_COPY_VALUE(&action_name, &tmp);
	}

	DAO_MM_ZVAL_STRING(&converter_key, "namespace");
	DAO_MM_CALL_SELF(&converter, "getconverter", &converter_key);

	if (dao_is_callable(&converter)) {
		zval tmp = {};

		array_init_size(&parameters, 1);
		DAO_MM_ADD_ENTRY(&parameters);
		dao_array_append(&parameters, &namespace_name, PH_COPY);
		DAO_MM_CALL_USER_FUNC_ARRAY(&tmp, &converter, &parameters);
		DAO_MM_ADD_ENTRY(&tmp);
		ZVAL_COPY_VALUE(&namespace_name, &tmp);
	}

	dao_read_property(&lower_case, getThis(), SL("_lowerCase"), PH_NOISY|PH_READONLY);

	if (zend_is_true(&lower_case)) {
		dao_fast_strtolower(&lower_controller_name, &controller_name);
		dao_fast_strtolower(&lower_action_name, &action_name);
		DAO_MM_ADD_ENTRY(&lower_controller_name);
		DAO_MM_ADD_ENTRY(&lower_action_name);
	} else {
		ZVAL_COPY_VALUE(&lower_controller_name, &controller_name);
		ZVAL_COPY_VALUE(&lower_action_name, &action_name);
	}

	/**
	 * Check if there is a layouts directory set
	 */
	dao_read_property(&layouts_dir, getThis(), SL("_layoutsDir"), PH_COPY);
	if (!zend_is_true(&layouts_dir)) {
		DAO_MM_ZVAL_STRING(&layouts_dir, "layouts/");
	}

	dao_read_property(&enable_namespace_view, getThis(), SL("_enableNamespaceView"), PH_NOISY|PH_READONLY);

	if (zend_is_true(&enable_namespace_view)) {
		zval lower_namespace_name = {};
		if (zend_is_true(&lower_case)) {
			dao_fast_strtolower(&lower_namespace_name, &namespace_name);
			DAO_MM_ADD_ENTRY(&lower_namespace_name);
		} else {
			ZVAL_COPY_VALUE(&lower_namespace_name, &namespace_name);
		}

		DAO_STR_REPLACE(&ds_lower_namespace_name, &namespace_separator, &ds, &lower_namespace_name);
		DAO_MM_ADD_ENTRY(&ds_lower_namespace_name);
		DAO_CONCAT_SV(&layout_namespace, "namespace/", &ds_lower_namespace_name);
		DAO_MM_ADD_ENTRY(&layout_namespace);
	}

	/**
	 * Check if the user has defined a custom layout
	 */
	dao_read_property(&layout, getThis(), SL("_layout"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&layout)) {
		ZVAL_COPY_VALUE(&layout_name, &layout);
	} else if (DAO_IS_NOT_EMPTY(&ds_lower_namespace_name)) {
		DAO_CONCAT_VSV(&layout_name, &ds_lower_namespace_name, "/", &lower_controller_name);
		DAO_MM_ADD_ENTRY(&layout_name);
	} else {
		ZVAL_COPY_VALUE(&layout_name, &lower_controller_name);
	}

	/**
	 * Load the template engines
	 */
	DAO_MM_CALL_METHOD(&engines, getThis(), "_loadtemplateengines");
	DAO_MM_ADD_ENTRY(&engines);

	/**
	 * Check if the user has picked a view diferent than the automatic
	 */
	dao_read_property(&pick_view, getThis(), SL("_pickView"), PH_READONLY);
	if (Z_TYPE(pick_view) == IS_NULL) {
		if (DAO_IS_NOT_EMPTY(&ds_lower_namespace_name)) {
			DAO_CONCAT_VSVSV(&render_view, &ds_lower_namespace_name, "/", &lower_controller_name, "/", &lower_action_name);
		} else {
			DAO_CONCAT_VSV(&render_view, &lower_controller_name, "/", &lower_action_name);
		}
		DAO_MM_ADD_ENTRY(&render_view);
	} else {
		/**
		 * The 'picked' view is an array, where the first element is controller and the
		 * second the action
		 */
		dao_array_fetch_long(&render_view, &pick_view, 0, PH_NOISY|PH_COPY);
		if (dao_array_isset_fetch_long(&pick_view_action, &pick_view, 1, PH_READONLY)) {
			ZVAL_COPY_VALUE(&layout_name, &pick_view_action);
		}
	}

	/**
	 * Call beforeRender if there is an events manager
	 */
	DAO_MM_ZVAL_STRING(&event_name, "view:beforeRender");
	DAO_MM_CALL_METHOD(&status, getThis(), "fireeventcancel", &event_name);
	if (DAO_IS_FALSE(&status)) {
		RETURN_MM_FALSE;
	}
	zval_ptr_dtor(&status);

	/**
	 * Get the current content in the buffer maybe some output from the controller
	 */

	if (dao_ob_get_level() >= 1) {
		dao_ob_get_contents(&contents);
		dao_ob_clean();
		DAO_MM_ADD_ENTRY(&contents);
		if (Z_TYPE(contents) == IS_STRING) {
			DAO_MM_CALL_METHOD(NULL, getThis(), "setcontent", &contents, &DAO_GLOBAL(z_true));
		}
	}
	ZVAL_TRUE(&silence);

	/**
	 * Disabled levels allow to avoid an specific level of rendering
	 */
	dao_read_property(&disabled_levels, getThis(), SL("_disabledLevels"), PH_NOISY|PH_READONLY);

	/**
	 * Render level will tell use when to stop
	 */
	dao_read_property(&render_level, getThis(), SL("_renderLevel"), PH_NOISY|PH_READONLY);
	if (zend_is_true(&render_level)) {
		dao_read_property(&enable_layouts_absolute_path, getThis(), SL("_enableLayoutsAbsolutePath"), PH_READONLY);

		if (view_model && Z_TYPE_P(view_model) == IS_OBJECT && zend_class_implements_interface(Z_OBJCE_P(view_model), dao_mvc_view_modelinterface_ce)) {
			zval model_content = {};
			DAO_MM_CALL_METHOD(NULL, view_model, "setview", getThis());
			DAO_MM_CALL_METHOD(&model_content, view_model, "render");
			dao_update_property(getThis(), SL("_content"), &model_content);
			zval_ptr_dtor(&model_content);
		}

		if (DAO_GE_LONG(&render_level, DAO_VIEW_LEVEL_ACTION)) {
			/**
			 * Inserts view related to action
			 */
			if (!dao_array_isset_long(&disabled_levels, DAO_VIEW_LEVEL_ACTION)) {
				if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
					ZVAL_STRING(&debug_message, "Ready insert action view");
					DAO_DEBUG_LOG(&debug_message);
					zval_ptr_dtor(&debug_message);
				}
				dao_update_property_long(getThis(), SL("_currentRenderLevel"), DAO_VIEW_LEVEL_ACTION);
				DAO_MM_CALL_METHOD(NULL, getThis(), "_enginerender", &engines, &render_view, &silence, &DAO_GLOBAL(z_true));
			}
		}

		/**
		 * Inserts templates before layout
		 */
		if (DAO_GE_LONG(&render_level, DAO_VIEW_LEVEL_BEFORE_TEMPLATE)) {
			if (!dao_array_isset_long(&disabled_levels, DAO_VIEW_LEVEL_BEFORE_TEMPLATE)) {
				if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
					ZVAL_STRING(&debug_message, "Ready insert templates before layout");
					DAO_DEBUG_LOG(&debug_message);
					zval_ptr_dtor(&debug_message);
				}
				dao_update_property_long(getThis(), SL("_currentRenderLevel"), DAO_VIEW_LEVEL_BEFORE_TEMPLATE);

				dao_read_property(&templates_before, getThis(), SL("_templatesBefore"), PH_NOISY|PH_READONLY);

				/**
				 * Templates before must be an array
				 */
				if (DAO_IS_NOT_EMPTY_ARR(&templates_before)) {

					ZVAL_FALSE(&silence);

					ZEND_HASH_FOREACH_VAL(Z_ARRVAL(templates_before), tpl) {
						DAO_CONCAT_VV(&view_tpl_path, &layouts_dir, tpl);
						DAO_MM_ADD_ENTRY(&view_tpl_path);
						DAO_MM_CALL_METHOD(NULL, getThis(), "_enginerender", &engines, &view_tpl_path, &silence, &DAO_GLOBAL(z_true), &enable_layouts_absolute_path);
					} ZEND_HASH_FOREACH_END();

					ZVAL_TRUE(&silence);
				} else if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
					ZVAL_STRING(&debug_message, "--Not set");
					DAO_DEBUG_LOG(&debug_message);
					zval_ptr_dtor(&debug_message);
				}
			}
		}

		/**
		 * Inserts controller layout
		 */
		if (DAO_GE_LONG(&render_level, DAO_VIEW_LEVEL_CONTROLLER)) {
			if (!dao_array_isset_long(&disabled_levels, DAO_VIEW_LEVEL_CONTROLLER)) {
				if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
					ZVAL_STRING(&debug_message, "Ready insert controller layout");
					DAO_DEBUG_LOG(&debug_message);
					zval_ptr_dtor(&debug_message);
				}
				dao_update_property_long(getThis(), SL("_currentRenderLevel"), DAO_VIEW_LEVEL_CONTROLLER);

				DAO_CONCAT_VV(&view_tpl_path, &layouts_dir, &layout_name);
				DAO_MM_ADD_ENTRY(&view_tpl_path);
				DAO_MM_CALL_METHOD(NULL, getThis(), "_enginerender", &engines, &view_tpl_path, &silence, &DAO_GLOBAL(z_true), &enable_layouts_absolute_path);
			}
		}

		/**
		 * Inserts namespace layout
		 */
		if (DAO_GE_LONG(&render_level, DAO_VIEW_LEVEL_NAMESPACE) && DAO_IS_NOT_EMPTY(&layout_namespace)) {
			if (!dao_array_isset_long(&disabled_levels, DAO_VIEW_LEVEL_NAMESPACE)) {
				zval enable_multi_namespace_view = {}, pos = {};
				if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
					ZVAL_STRING(&debug_message, "Ready insert namespace layout");
					DAO_DEBUG_LOG(&debug_message);
					zval_ptr_dtor(&debug_message);
				}
				dao_update_property_long(getThis(), SL("_currentRenderLevel"), DAO_VIEW_LEVEL_NAMESPACE);

				DAO_CONCAT_VV(&view_tpl_path, &layouts_dir, &layout_namespace);
				DAO_MM_ADD_ENTRY(&view_tpl_path);
				DAO_MM_CALL_METHOD(NULL, getThis(), "_enginerender", &engines, &view_tpl_path, &silence, &DAO_GLOBAL(z_true), &enable_layouts_absolute_path);

				dao_read_property(&enable_multi_namespace_view, getThis(), SL("_enableMultiNamespaceView"), PH_NOISY|PH_READONLY);
				if (zend_is_true(&enable_multi_namespace_view)) {
					/**
					 * Top-level namespace
					 */
					while (dao_fast_strrpos(&pos, &layout_namespace, &ds)) {
						zval tmp = {};

						dao_substr(&tmp, &layout_namespace, 0, Z_LVAL(pos));
						DAO_MM_ADD_ENTRY(&tmp);
						ZVAL_COPY_VALUE(&layout_namespace, &tmp);

						DAO_CONCAT_VV(&view_tpl_path, &layouts_dir, &layout_namespace);
						DAO_MM_ADD_ENTRY(&view_tpl_path);
						DAO_CALL_METHOD(NULL, getThis(), "_enginerender", &engines, &view_tpl_path, &silence, &DAO_GLOBAL(z_true), &enable_layouts_absolute_path);
					}
				}
			}
		}

		/**
		 * Inserts templates after layout
		 */
		if (DAO_GE_LONG(&render_level, DAO_VIEW_LEVEL_AFTER_TEMPLATE)) {
			if (!dao_array_isset_long(&disabled_levels, DAO_VIEW_LEVEL_AFTER_TEMPLATE)) {
				if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
					ZVAL_STRING(&debug_message, "Ready inserts templates after layout");
					DAO_DEBUG_LOG(&debug_message);
					zval_ptr_dtor(&debug_message);
				}
				dao_update_property_long(getThis(), SL("_currentRenderLevel"), DAO_VIEW_LEVEL_AFTER_TEMPLATE);

				/**
				 * Templates after must be an array
				 */
				dao_read_property(&templates_after, getThis(), SL("_templatesAfter"), PH_NOISY|PH_READONLY);
				if (DAO_IS_NOT_EMPTY_ARR(&templates_after)) {
					ZVAL_FALSE(&silence);

					ZEND_HASH_FOREACH_VAL(Z_ARRVAL(templates_after), tpl) {
						DAO_CONCAT_VV(&view_tpl_path, &layouts_dir, tpl);
						DAO_MM_ADD_ENTRY(&view_tpl_path);
						DAO_MM_CALL_METHOD(NULL, getThis(), "_enginerender", &engines, &view_tpl_path, &silence, &DAO_GLOBAL(z_true), &enable_layouts_absolute_path);
					} ZEND_HASH_FOREACH_END();

					ZVAL_TRUE(&silence);
				} else if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
					ZVAL_STRING(&debug_message, "--Not set");
					DAO_DEBUG_LOG(&debug_message);
					zval_ptr_dtor(&debug_message);
				}
			}
		}

		/**
		 * Inserts main view
		 */
		if (DAO_GE_LONG(&render_level, DAO_VIEW_LEVEL_MAIN)) {
			if (!dao_array_isset_long(&disabled_levels, DAO_VIEW_LEVEL_MAIN)) {
				if (unlikely(DAO_GLOBAL(debug).enable_debug)) {
					ZVAL_STRING(&debug_message, "Ready insert main view");
					DAO_DEBUG_LOG(&debug_message);
					zval_ptr_dtor(&debug_message);
				}
				dao_update_property_long(getThis(), SL("_currentRenderLevel"), DAO_VIEW_LEVEL_MAIN);

				dao_read_property(&main_view, getThis(), SL("_mainView"), PH_NOISY|PH_READONLY);
				DAO_MM_CALL_METHOD(NULL, getThis(), "_enginerender", &engines, &main_view, &silence, &DAO_GLOBAL(z_true));
			}
		}

		dao_update_property(getThis(), SL("_currentRenderLevel"), &DAO_GLOBAL(z_zero));
	}

	/**
	 * Call afterRender event
	 */
	DAO_MM_ZVAL_STRING(&event_name, "view:afterRender");
	DAO_MM_CALL_METHOD(NULL, getThis(), "fireevent", &event_name);

	RETURN_MM_THIS();
}

/**
 * Choose a different view to render instead of last-controller/last-action
 *
 * <code>
 * class ProductsController extends Dao\Mvc\Controller
 * {
 *
 *    public function saveAction()
 *    {
 *
 *         //Do some save stuff...
 *
 *         //Then show the list view
 *         $this->view->pick("products/list");
 *    }
 * }
 * </code>
 *
 * @param string|array $renderView
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, pick){

	zval *render_view, pick_view = {}, parts = {}, layout = {};

	dao_fetch_params(1, 1, 0, &render_view);

	if (Z_TYPE_P(render_view) == IS_ARRAY) {
		ZVAL_COPY_VALUE(&pick_view, render_view);
	} else {
		array_init_size(&pick_view, 2);
		DAO_MM_ADD_ENTRY(&pick_view);
		dao_array_append(&pick_view, render_view, PH_COPY);

		if (dao_memnstr_str(render_view, SL("/"))) {
			dao_fast_explode_str(&parts, SL("/"), render_view);
			dao_array_fetch_long(&layout, &parts, 0, PH_NOISY|PH_READONLY);
			dao_array_append(&pick_view, &layout, PH_COPY);
			zval_ptr_dtor(&parts);
		}
	}
	dao_update_property(getThis(), SL("_pickView"), &pick_view);

	RETURN_MM_THIS();
}

/**
 * Renders a partial view
 *
 * <code>
 * 	//Show a partial inside another view
 * 	$this->partial('shared/footer');
 * </code>
 *
 * <code>
 * 	//Show a partial inside another view with parameters
 * 	$this->partial('shared/footer', array('content' => $html));
 * </code>
 *
 * @param string $partialPath
 * @param array $params
 * @param boolean $autorender
 */
PHP_METHOD(Dao_Mvc_View, partial){

	zval *partial_path, *params = NULL, *autorender = NULL, view_params = {}, new_params = {}, partials_dir = {}, enable_partials_absolute_path = {};
	zval real_path = {}, engines = {};

	dao_fetch_params(1, 1, 2, &partial_path, &params, &autorender);

	if (!params) {
		params = &DAO_GLOBAL(z_null);
	}

	if (!autorender) {
		autorender = &DAO_GLOBAL(z_true);
	}

	/**
	 * If the developer pass an array of variables we create a new virtual symbol table
	 */
	if (Z_TYPE_P(params) == IS_ARRAY) {
		dao_read_property(&view_params, getThis(), SL("_viewParams"), PH_NOISY|PH_COPY);

		/**
		 * Merge or assign the new params as parameters
		 */
		if (Z_TYPE(view_params) == IS_ARRAY) {
			dao_fast_array_merge(&new_params, &view_params, params);
			DAO_MM_ADD_ENTRY(&new_params);
		} else {
			ZVAL_COPY_VALUE(&new_params, params);
		}

		/**
		 * Update the parameters with the new ones
		 */
		dao_update_property(getThis(), SL("_viewParams"), &new_params);
	}

	dao_read_property(&partials_dir, getThis(), SL("_partialsDir"), PH_NOISY|PH_READONLY);
	dao_read_property(&enable_partials_absolute_path, getThis(), SL("_enablePartialsAbsolutePath"), PH_NOISY|PH_READONLY);

	/**
	 * Partials are looked up under the partials directory
	 */
	DAO_CONCAT_VV(&real_path, &partials_dir, partial_path);
	DAO_MM_ADD_ENTRY(&real_path);

	/**
	 * We need to check if the engines are loaded first, this method could be called
	 * outside of 'render'
	 */
	DAO_MM_CALL_METHOD(&engines, getThis(), "_loadtemplateengines");
	DAO_MM_ADD_ENTRY(&engines);

	/**
	 * Call engine render, this checks in every registered engine for the partial
	 */
	if (!DAO_IS_TRUE(autorender)) {
		dao_ob_start();
		DAO_MM_CALL_METHOD(NULL, getThis(), "_enginerender", &engines, &real_path, &DAO_GLOBAL(z_false), &DAO_GLOBAL(z_false), &enable_partials_absolute_path);
		dao_ob_get_contents(return_value);
		dao_ob_clean();

	} else {
		DAO_MM_CALL_METHOD(NULL, getThis(), "_enginerender", &engines, &real_path, &DAO_GLOBAL(z_false), &DAO_GLOBAL(z_false), &enable_partials_absolute_path);
	}

	/**
	 * Now we need to restore the original view parameters
	 */
	if (Z_TYPE_P(params) == IS_ARRAY) {
		/**
		 * Restore the original view params
		 */
		dao_update_property(getThis(), SL("_viewParams"), &view_params);
	}

	RETURN_MM();
}

/**
 * Perform the automatic rendering returning the output as a string
 *
 * <code>
 * 	$template = $this->view->getRender('products', 'show', array('products' => $products));
 * </code>
 *
 * @param string $controllerName
 * @param string $actionName
 * @param array $params
 * @param mixed $configCallback
 * @return string
 */
PHP_METHOD(Dao_Mvc_View, getRender){

	zval *controller_name, *action_name, *params = NULL, *config_callback = NULL, view = {}, params_tmp = {};

	dao_fetch_params(1, 2, 2, &controller_name, &action_name, &params, &config_callback);

	if (!params) {
		params = &DAO_GLOBAL(z_null);
	}

	if (!config_callback) {
		config_callback = &DAO_GLOBAL(z_null);
	}

	/**
	 * We must to clone the current view to keep the old state
	 */
	if (dao_clone(&view, getThis()) == FAILURE) {
		RETURN_MM();
	}

	DAO_MM_ADD_ENTRY(&view);

	/**
	 * The component must be reset to its defaults
	 */
	DAO_MM_CALL_METHOD(NULL, &view, "reset");

	/**
	 * Set the render variables
	 */
	if (Z_TYPE_P(params) == IS_ARRAY) {
		DAO_MM_CALL_METHOD(NULL, &view, "setvars", params);
	}

	/**
	 * Perform extra configurations over the cloned object
	 */
	if (Z_TYPE_P(config_callback) == IS_OBJECT) {
		array_init_size(&params_tmp, 1);
		DAO_MM_ADD_ENTRY(&params_tmp);
		dao_array_append(&params_tmp, &view, PH_COPY);

		DAO_MM_CALL_USER_FUNC_ARRAY(NULL, config_callback, &params_tmp);
	}

	/**
	 * Start the output buffering
	 */
	DAO_MM_CALL_METHOD(NULL, &view, "start");

	/**
	 * Perform the render passing only the controller and action
	 */
	DAO_MM_CALL_METHOD(NULL, &view, "render", controller_name, action_name);

	/**
	 * Stop the output buffering
	 */
	dao_ob_end_clean();

	/**
	 * Get the processed content
	 */
	DAO_MM_RETURN_CALL_METHOD(&view, "getcontent");
	RETURN_MM();
}

/**
 * Finishes the render process by stopping the output buffering
 *
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, finish){

	dao_ob_end_clean();
	RETURN_THIS();
}

/**
 * Create a Dao\Cache based on the internal cache options
 *
 * @return Dao\Cache\BackendInterface
 */
PHP_METHOD(Dao_Mvc_View, _createCache){

	zval dependency_injector = {}, view_options = {}, cache_options = {}, cache_service = {};

	DAO_MM_INIT();
	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);
	if (Z_TYPE(dependency_injector) != IS_OBJECT) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_view_exception_ce, "A dependency injector container is required to obtain the view cache services");
		return;
	}

	DAO_MM_ZVAL_STRING(&cache_service, "viewCache");

	dao_read_property(&view_options, getThis(), SL("_options"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(view_options) == IS_ARRAY) {
		if (dao_array_isset_fetch_str(&cache_options, &view_options, SL("cache"), PH_READONLY)) {
			if (Z_TYPE(cache_options) == IS_ARRAY && dao_array_isset_str(&cache_options, SL("service"))) {
				dao_array_fetch_str(&cache_service, &cache_options, SL("service"), PH_NOISY|PH_READONLY);
			}
		}
	}

	/**
	 * The injected service must be an object
	 */
	DAO_MM_CALL_METHOD(return_value, &dependency_injector, "getshared", &cache_service);
	if (Z_TYPE_P(return_value) != IS_OBJECT) {
		DAO_MM_THROW_EXCEPTION_STR(dao_mvc_view_exception_ce, "The injected caching service is invalid");
		return;
	}

	DAO_MM_VERIFY_INTERFACE(return_value, dao_cache_backendinterface_ce);
	RETURN_MM();
}

/**
 * Check if the component is currently caching the output content
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_View, isCaching){

	zval cache_level = {};

	dao_read_property(&cache_level, getThis(), SL("_cacheLevel"), PH_NOISY|PH_READONLY);
	is_smaller_function(return_value, &DAO_GLOBAL(z_zero), &cache_level);
}

/**
 * Returns the cache instance used to cache
 *
 * @return Dao\Cache\BackendInterface
 */
PHP_METHOD(Dao_Mvc_View, getCache){

	zval cache = {};

	DAO_MM_INIT();
	dao_read_property(&cache, getThis(), SL("_cache"), PH_READONLY);
	if (zend_is_true(&cache)) {
		if (Z_TYPE(cache) != IS_OBJECT) {
			DAO_CALL_METHOD(&cache, getThis(), "_createcache");
			DAO_MM_ADD_ENTRY(&cache);
			dao_update_property(getThis(), SL("_cache"), &cache);

		}
	} else {
		DAO_MM_CALL_METHOD(&cache, getThis(), "_createcache");
		DAO_MM_ADD_ENTRY(&cache);
		dao_update_property(getThis(), SL("_cache"), &cache);
	}

	RETURN_MM_CTOR(&cache);
}

/**
 * Cache the actual view render to certain level
 *
 *<code>
 *  $this->view->cache(array('key' => 'my-key', 'lifetime' => 86400));
 *</code>
 *
 * @param boolean|array $options
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, cache){

	zval *options = NULL, view_options = {}, cache_options = {}, *value, cache_level = {}, cache_mode = {};
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(1, 0, 1, &options);

	if (!options) {
		options = &DAO_GLOBAL(z_true);
	}

	if (Z_TYPE_P(options) == IS_ARRAY) {
		dao_read_property(&view_options, getThis(), SL("_options"), PH_READONLY);
		if (Z_TYPE(view_options) != IS_ARRAY) {
			array_init(&view_options);
			DAO_MM_ADD_ENTRY(&view_options);
		}

		/**
		 * Get the default cache options
		 */
		if (!dao_array_isset_fetch_str(&cache_options, &view_options, SL("cache"), PH_READONLY)) {
			array_init(&cache_options);
			DAO_MM_ADD_ENTRY(&cache_options);
		}

		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(options), idx, str_key, value) {
			if (str_key) {
				dao_array_update_string(&cache_options, str_key, value, PH_COPY);
			} else {
				dao_array_update_long(&cache_options, idx, value, PH_COPY);
			}

		} ZEND_HASH_FOREACH_END();

		/**
		 * Check if the user has defined a default cache level or use 5 as default
		 */
		if (dao_array_isset_fetch_str(&cache_level, &cache_options, SL("level"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_cacheLevel"), &cache_level);
		} else {
			dao_update_property_long(getThis(), SL("_cacheLevel"), 5);
		}

		if (dao_array_isset_fetch_str(&cache_mode, &cache_options, SL("mode"), PH_READONLY)) {
			dao_update_property(getThis(), SL("_cacheMode"), &cache_mode);
		} else {
			dao_update_property_bool(getThis(), SL("_cacheMode"), 0);
		}

		dao_array_update_str(&view_options, SL("cache"), &cache_options, PH_COPY);
		dao_update_property(getThis(), SL("_options"), &view_options);
	} else {
		/**
		 * If 'options' isn't an array we enable the cache with the default options
		 */
		if (zend_is_true(options)) {
			dao_update_property_long(getThis(), SL("_cacheLevel"), 5);
		} else {
			dao_update_property_long(getThis(), SL("_cacheLevel"), 0);
		}

		dao_update_property_bool(getThis(), SL("_cacheMode"), 0);
	}

	RETURN_MM_THIS();
}

/**
 * Externally sets the view content
 *
 *<code>
 *	$this->view->setContent("<h1>hello</h1>");
 *</code>
 *
 * @param string $content
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setContent){

	zval *content, *append = NULL;

	dao_fetch_params(0, 1, 1, &content, &append);

	if (Z_TYPE_P(content) != IS_STRING) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_view_exception_ce, "Content must be a string");
		return;
	}

	if (append && Z_TYPE_P(append) == IS_TRUE) {
		zval old_content = {}, new_content = {};
		dao_read_property(&old_content, getThis(), SL("_content"), PH_NOISY|PH_READONLY);
		DAO_CONCAT_VV(&new_content, &old_content, content);
		dao_update_property(getThis(), SL("_content"), &new_content);
		zval_ptr_dtor(&new_content);
	} else {
		dao_update_property(getThis(), SL("_content"), content);
	}

	RETURN_THIS();
}

/**
 * Returns cached output from another view stage
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View, getContent){


	RETURN_MEMBER(getThis(), "_content");
}

/**
 * Start a new section block
 *
 * @param string $name
 */
PHP_METHOD(Dao_Mvc_View, startSection){

	zval *name;

	dao_fetch_params(0, 1, 0, &name);

	dao_update_property_array(getThis(), SL("_sections"), name, &DAO_GLOBAL(z_null));
	dao_ob_start();
	RETURN_THIS();
}

/**
 * Stop the current section block
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View, stopSection){

	zval content = {}, sections = {}, name = {};
	HashTable *array;

	dao_ob_get_clean(&content);

	dao_read_property(&sections, getThis(), SL("_sections"), PH_NOISY|PH_READONLY);

	array = Z_ARRVAL(sections);

	zend_hash_internal_pointer_end(array);
	zend_hash_get_current_key_zval(array, &name);

	dao_update_property_array(getThis(), SL("_sections"), &name, &content);
	zval_ptr_dtor(&name);
	zval_ptr_dtor(&content);
	RETURN_THIS();
}

/**
 * Returns the content for a section block
 *
 * @param string $name
 * @param string $default
 * @return string|null
 */
PHP_METHOD(Dao_Mvc_View, section){

	zval *name, *default_value = NULL;

	dao_fetch_params(0, 1, 1, &name, &default_value);

	if (dao_isset_property_array(getThis(), SL("_sections"), name)) {
		dao_read_property_array(return_value, getThis(), SL("_sections"), name, PH_COPY);
	} else if (default_value) {
		RETURN_CTOR(default_value);
	} else {
		RETURN_NULL();
	}
}

/**
 * Returns the path of the view that is currently rendered
 *
 * @return string
 */
PHP_METHOD(Dao_Mvc_View, getActiveRenderPath){


	RETURN_MEMBER(getThis(), "_activeRenderPath");
}

/**
 * Disables the auto-rendering process
 *
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, disable){


	dao_update_property_bool(getThis(), SL("_disabled"), 1);
	RETURN_THIS();
}

/**
 * Enables the auto-rendering process
 *
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, enable){


	dao_update_property_bool(getThis(), SL("_disabled"), 0);
	RETURN_THIS();
}

/**
 * Whether automatic rendering is enabled
 *
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_View, isDisabled){

	RETURN_MEMBER(getThis(), "_disabled");
}

/**
 * Enables namespace view render
 *
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, enableNamespaceView){

	dao_update_property_bool(getThis(), SL("_enableNamespaceView"), 1);
	RETURN_THIS();
}

/**
 * Disables namespace view render
 *
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, disableNamespaceView){

	dao_update_property_bool(getThis(), SL("_enableNamespaceView"), 0);
	RETURN_THIS();
}

/**
 * Enables multi namespace view render
 *
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, enableMultiNamespaceView){

	dao_update_property_bool(getThis(), SL("_enableMultiNamespaceView"), 1);
	RETURN_THIS();
}

/**
 * Disables multi namespace view render
 *
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, disableMultiNamespaceView){

	dao_update_property_bool(getThis(), SL("_enableMultiNamespaceView"), 0);
	RETURN_THIS();
}

/**
 * Enables to lower case view path
 *
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, enableLowerCase){

	dao_update_property_bool(getThis(), SL("_lowerCase"), 1);
	RETURN_THIS();
}

/**
 * Whether to lower case view path
 *
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, disableLowerCase){

	dao_update_property_bool(getThis(), SL("_lowerCase"), 0);
	RETURN_THIS();
}

/**
 * Adds a converter
 *
 * @param string $name
 * @param callable $converter
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, setConverter){

	zval *name, *converter;

	dao_fetch_params(0, 2, 0, &name, &converter);

	if (!dao_is_callable(converter)) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_view_exception_ce, "The paramter `converter` is not callable");
		return;
	}

	dao_update_property_array(getThis(), SL("_converters"), name, converter);
	RETURN_THIS();
}

/**
 * Returns the router converter
 *
 * @return callable|null
 */
PHP_METHOD(Dao_Mvc_View, getConverter) {

	zval *name, converters = {}, converter = {};

	dao_fetch_params(0, 1, 0, &name);

	dao_read_property(&converters, getThis(), SL("_converters"), PH_NOISY|PH_READONLY);

	if (dao_array_isset_fetch(&converter, &converters, name, PH_READONLY)) {
		RETURN_CTOR(&converter);
	}

	RETURN_NULL();
}

/**
 * Resets the view component to its factory default values
 *
 * @return Dao\Mvc\View
 */
PHP_METHOD(Dao_Mvc_View, reset){

	dao_update_property(getThis(), SL("_disabled"), &DAO_GLOBAL(z_false));
	dao_update_property(getThis(), SL("_engines"), &DAO_GLOBAL(z_false));
	dao_update_property(getThis(), SL("_cache"), &DAO_GLOBAL(z_null));
	dao_update_property_long(getThis(), SL("_renderLevel"), 5);
	dao_update_property(getThis(), SL("_cacheLevel"), &DAO_GLOBAL(z_zero));
	dao_update_property(getThis(), SL("_content"), &DAO_GLOBAL(z_null));
	dao_update_property_empty_array(getThis(), SL("_sections"));
	dao_update_property(getThis(), SL("_templatesBefore"), &DAO_GLOBAL(z_null));
	dao_update_property(getThis(), SL("_templatesAfter"), &DAO_GLOBAL(z_null));
	RETURN_THIS();
}

/**
 * Magic method to pass variables to the views
 *
 *<code>
 *	$this->view->products = $products;
 *</code>
 *
 * @param string $key
 * @param mixed $value
 */
PHP_METHOD(Dao_Mvc_View, __set){

	zval *key, *value;

	dao_fetch_params(0, 2, 0, &key, &value);

	dao_update_property_array(getThis(), SL("_viewParams"), key, value);

}

/**
 * Magic method to retrieve a variable passed to the view
 *
 *<code>
 *	echo $this->view->products;
 *</code>
 *
 * @param string $key
 * @return mixed
 */
PHP_METHOD(Dao_Mvc_View, __get){

	zval *key, params = {}, value = {};

	dao_fetch_params(0, 1, 0, &key);

	dao_read_property(&params, getThis(), SL("_viewParams"), PH_NOISY|PH_READONLY);
	if (dao_array_isset_fetch(&value, &params, key, PH_READONLY)) {
		RETURN_CTOR(&value);
	}

	RETURN_NULL();
}

/**
 * Magic method to inaccessible a variable passed to the view
 *
 *<code>
 *	isset($this->view->products)
 *</code>
 *
 * @param string $key
 * @return boolean
 */
PHP_METHOD(Dao_Mvc_View, __isset){

	zval *key, params = {};

	dao_fetch_params(0, 1, 0, &key);

	dao_read_property(&params, getThis(), SL("_viewParams"), PH_NOISY|PH_READONLY);
	if (dao_array_isset(&params, key)) {
		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * Enables/disables options in the View
 *
 *<code>
 *	\Dao\Mvc\View::setup(['strict' => false]);
 *</code>
 *
 * @param array $options
 * @return void
 */
PHP_METHOD(Dao_Mvc_View, setup){

	zval *options, enable_strict = {};

	dao_fetch_params(0, 1, 0, &options);

	if (Z_TYPE_P(options) != IS_ARRAY) {
		DAO_THROW_EXCEPTION_STR(dao_mvc_view_exception_ce, "Options must be an array");
		return;
	}

	/**
	 * Enables/Disables strict mode
	 */
	if (dao_array_isset_fetch_str(&enable_strict, options, SL("strict"), PH_READONLY)) {
		DAO_GLOBAL(mvc).enable_view_strict = zend_is_true(&enable_strict);
	}

}
