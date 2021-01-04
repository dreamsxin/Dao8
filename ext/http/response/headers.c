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

#include "http/response/headers.h"
#include "http/response/headersinterface.h"

#include <main/SAPI.h>

#include <Zend/zend_smart_str.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/object.h"
#include "kernel/array.h"
#include "kernel/fcall.h"
#include "kernel/hash.h"
#include "kernel/operators.h"
#include "kernel/concat.h"
#include "kernel/debug.h"

/**
 * Dao\Http\Response\Headers
 *
 * This class is a bag to manage the response headers
 */
zend_class_entry *dao_http_response_headers_ce;

PHP_METHOD(Dao_Http_Response_Headers, set);
PHP_METHOD(Dao_Http_Response_Headers, get);
PHP_METHOD(Dao_Http_Response_Headers, setRaw);
PHP_METHOD(Dao_Http_Response_Headers, remove);
PHP_METHOD(Dao_Http_Response_Headers, send);
PHP_METHOD(Dao_Http_Response_Headers, reset);
PHP_METHOD(Dao_Http_Response_Headers, toArray);
PHP_METHOD(Dao_Http_Response_Headers, toString);
PHP_METHOD(Dao_Http_Response_Headers, __set_state);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_response_headers___set_state, 0, 0, 1)
	ZEND_ARG_INFO(0, data)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_http_response_headers_method_entry[] = {
	PHP_ME(Dao_Http_Response_Headers, set, arginfo_dao_http_response_headersinterface_set, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Headers, get, arginfo_dao_http_response_headersinterface_get, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Headers, setRaw, arginfo_dao_http_response_headersinterface_setraw, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Headers, remove, arginfo_dao_http_response_headersinterface_remove, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Headers, send, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Headers, reset, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Headers, toArray, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Headers, toString, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Response_Headers, __set_state, arginfo_dao_http_response_headers___set_state, ZEND_ACC_PUBLIC|ZEND_ACC_STATIC)
	PHP_FE_END
};

/**
 * Dao\Http\Response\Headers initializer
 */
DAO_INIT_CLASS(Dao_Http_Response_Headers){

	DAO_REGISTER_CLASS(Dao\\Http\\Response, Headers, http_response_headers, dao_http_response_headers_method_entry, 0);

	zend_declare_property_null(dao_http_response_headers_ce, SL("_headers"), ZEND_ACC_PROTECTED);

	zend_class_implements(dao_http_response_headers_ce, 1, dao_http_response_headersinterface_ce);

	return SUCCESS;
}

/**
 * Sets a header to be sent at the end of the request
 *
 * @param string $name
 * @param string $value
 */
PHP_METHOD(Dao_Http_Response_Headers, set){

	zval *name, *value;

	dao_fetch_params(0, 2, 0, &name, &value);
	dao_update_property_array(getThis(), SL("_headers"), name, value);
}

/**
 * Gets a header value from the internal bag
 *
 * @param string $name
 * @return string
 */
PHP_METHOD(Dao_Http_Response_Headers, get){

	zval *name, headers = {};

	dao_fetch_params(0, 1, 0, &name);

	dao_read_property(&headers, getThis(), SL("_headers"), PH_NOISY|PH_READONLY);
	if (!dao_array_isset_fetch(return_value, &headers, name, PH_COPY)) {
		RETURN_FALSE;
	}
}

/**
 * Sets a raw header to be sent at the end of the request
 *
 * @param string $header
 */
PHP_METHOD(Dao_Http_Response_Headers, setRaw){

	zval *header;

	dao_fetch_params(0, 1, 0, &header);
	DAO_SEPARATE_PARAM(header);

	dao_update_property_array(getThis(), SL("_headers"), header, &DAO_GLOBAL(z_null));
}

/**
 * Removes a header to be sent at the end of the request
 *
 * @param string $header Header name
 */
PHP_METHOD(Dao_Http_Response_Headers, remove){

	zval *header_index, headers = {};

	dao_fetch_params(0, 1, 0, &header_index);

	dao_read_property(&headers, getThis(), SL("_headers"), PH_NOISY|PH_READONLY);

	dao_array_unset(&headers, header_index, 0);
}

/**
 * Sends the headers to the client
 *
 * @return boolean
 */
PHP_METHOD(Dao_Http_Response_Headers, send)
{
	zval headers = {}, *value;
	sapi_header_line ctr = { NULL, 0, 0 };
	zend_string *str_key;
	ulong idx;

	if (!SG(headers_sent)) {
		dao_read_property(&headers, getThis(), SL("_headers"), PH_NOISY|PH_READONLY);

		if (Z_TYPE(headers) != IS_ARRAY) {
			/* No headers to send */
			RETURN_TRUE;
		}

		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(headers), idx, str_key, value) {
			zval header = {};
			if (str_key) {
				ZVAL_STR(&header, str_key);
			} else {
				ZVAL_LONG(&header, idx);
			}

			if (DAO_IS_NOT_EMPTY(value)) {
				zval http_header = {};
				DAO_CONCAT_VSV(&http_header, &header, ": ", value);
				ctr.line     = Z_STRVAL(http_header);
				ctr.line_len = Z_STRLEN(http_header);
				sapi_header_op(SAPI_HEADER_REPLACE, &ctr);
				zval_ptr_dtor(&http_header);
			} else if (Z_TYPE(header) == IS_STRING) {
				ctr.line     = Z_STRVAL(header);
				ctr.line_len = Z_STRLEN(header);
				sapi_header_op(SAPI_HEADER_REPLACE, &ctr);
			} else {
				zval tmp= {};
				ZVAL_DUP(&tmp, &header);
				convert_to_string(&tmp);

				ctr.line     = Z_STRVAL(tmp);
				ctr.line_len = Z_STRLEN(tmp);
				sapi_header_op(SAPI_HEADER_REPLACE, &ctr);
				zval_ptr_dtor(&tmp);
			}
		} ZEND_HASH_FOREACH_END();

		RETURN_TRUE;
	}

	RETURN_FALSE;
}

/**
 * Reset set headers
 *
 */
PHP_METHOD(Dao_Http_Response_Headers, reset){


	dao_update_property_empty_array(getThis(), SL("_headers"));
}

/**
 * Returns the current headers as an array
 *
 * @return array
 */
PHP_METHOD(Dao_Http_Response_Headers, toArray){


	RETURN_MEMBER(getThis(), "_headers");
}

/**
 * Returns the current headers as an string
 *
 * @return string
 */
PHP_METHOD(Dao_Http_Response_Headers, toString)
{
	zval headers = {}, *value;
	smart_str buffer = {0};
	zend_string *str_key;
	ulong idx;

	dao_read_property(&headers, getThis(), SL("_headers"), PH_NOISY|PH_READONLY);

	if (SG(sapi_headers).http_status_line) {
		smart_str_appends(&buffer, SG(sapi_headers).http_status_line);
		smart_str_appendl(&buffer, "\r\n", 2);
	}

	if (Z_TYPE(headers) == IS_ARRAY) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(headers), idx, str_key, value) {
			zval header = {}, http_header = {}, tmp = {};
			if (str_key) {
				ZVAL_STR(&header, str_key);
			} else {
				ZVAL_LONG(&header, idx);
			}

			if (DAO_IS_NOT_EMPTY(value)) {
				DAO_CONCAT_VSV(&http_header, &header, ": ", value);
				smart_str_appendl(&buffer, Z_STRVAL(http_header), Z_STRLEN(http_header));
			} else if (Z_TYPE(header) == IS_STRING) {
				smart_str_appendl(&buffer, Z_STRVAL(header), Z_STRLEN(header));
			} else {
				ZVAL_ZVAL(&tmp, &header, 1, 0);
				convert_to_string(&tmp);
				smart_str_appendl(&buffer, Z_STRVAL(tmp), Z_STRLEN(tmp));
			}
			smart_str_appendl(&buffer, "\r\n", 2);
		} ZEND_HASH_FOREACH_END();
	}
	smart_str_0(&buffer);

	RETURN_STR(buffer.s);
}

/**
 * Restore a Dao\Http\Response\Headers object
 *
 * @param array $data
 * @return Dao\Http\Response\Headers
 */
PHP_METHOD(Dao_Http_Response_Headers, __set_state){

	zval *data, headers = {}, data_headers = {}, *value;
	zend_string *str_key;
	ulong idx;

	dao_fetch_params(0, 1, 0, &data);

	object_init_ex(&headers, dao_http_response_headers_ce);
	if (dao_array_isset_fetch_str(&data_headers, data, SL("_headers"), PH_READONLY)) {
		ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL(data_headers), idx, str_key, value) {
			zval key = {};
			if (str_key) {
				ZVAL_STR(&key, str_key);
			} else {
				ZVAL_LONG(&key, idx);
			}
			DAO_CALL_METHOD(NULL, &headers, "set", &key, value);
		} ZEND_HASH_FOREACH_END();
	}

	RETURN_CTOR(&headers);
}
