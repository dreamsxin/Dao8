
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

#include "http/cookie.h"
#include "http/cookie/exception.h"
#include "cryptinterface.h"
#include "diinterface.h"
#include "di/injectable.h"
#include "filterinterface.h"
#include "session/adapterinterface.h"

#include <ext/standard/head.h>

#include "kernel/main.h"
#include "kernel/memory.h"
#include "kernel/exception.h"
#include "kernel/object.h"
#include "kernel/fcall.h"
#include "kernel/operators.h"
#include "kernel/array.h"
#include "kernel/file.h"
#include "kernel/concat.h"

#include "interned-strings.h"

/**
 * Dao\Http\Cookie
 *
 * Provide OO wrappers to manage a HTTP cookie
 */
zend_class_entry *dao_http_cookie_ce;

PHP_METHOD(Dao_Http_Cookie, __construct);
PHP_METHOD(Dao_Http_Cookie, setValue);
PHP_METHOD(Dao_Http_Cookie, getValue);
PHP_METHOD(Dao_Http_Cookie, send);
PHP_METHOD(Dao_Http_Cookie, restore);
PHP_METHOD(Dao_Http_Cookie, delete);
PHP_METHOD(Dao_Http_Cookie, useEncryption);
PHP_METHOD(Dao_Http_Cookie, isUsingEncryption);
PHP_METHOD(Dao_Http_Cookie, setExpiration);
PHP_METHOD(Dao_Http_Cookie, getExpiration);
PHP_METHOD(Dao_Http_Cookie, setPath);
PHP_METHOD(Dao_Http_Cookie, getPath);
PHP_METHOD(Dao_Http_Cookie, setDomain);
PHP_METHOD(Dao_Http_Cookie, getDomain);
PHP_METHOD(Dao_Http_Cookie, setSecure);
PHP_METHOD(Dao_Http_Cookie, getSecure);
PHP_METHOD(Dao_Http_Cookie, setHttpOnly);
PHP_METHOD(Dao_Http_Cookie, getHttpOnly);
PHP_METHOD(Dao_Http_Cookie, __toString);

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_cookie___construct, 0, 0, 1)
	ZEND_ARG_INFO(0, name)
	ZEND_ARG_INFO(0, value)
	ZEND_ARG_INFO(0, expire)
	ZEND_ARG_INFO(0, path)
	ZEND_ARG_INFO(0, secure)
	ZEND_ARG_INFO(0, domain)
	ZEND_ARG_INFO(0, httpOnly)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_cookie_setvalue, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_cookie_getvalue, 0, 0, 0)
	ZEND_ARG_INFO(0, filters)
	ZEND_ARG_INFO(0, defaultValue)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_cookie_useencryption, 0, 0, 1)
	ZEND_ARG_INFO(0, useEncryption)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_cookie_setexpiration, 0, 0, 1)
	ZEND_ARG_INFO(0, expire)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_cookie_setpath, 0, 0, 1)
	ZEND_ARG_INFO(0, path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_cookie_setdomain, 0, 0, 1)
	ZEND_ARG_INFO(0, domain)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_cookie_setsecure, 0, 0, 1)
	ZEND_ARG_INFO(0, secure)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(arginfo_dao_http_cookie_sethttponly, 0, 0, 1)
	ZEND_ARG_INFO(0, httpOnly)
ZEND_END_ARG_INFO()

static const zend_function_entry dao_http_cookie_method_entry[] = {
	PHP_ME(Dao_Http_Cookie, __construct, arginfo_dao_http_cookie___construct, ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
	PHP_ME(Dao_Http_Cookie, setValue, arginfo_dao_http_cookie_setvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, getValue, arginfo_dao_http_cookie_getvalue, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, send, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, restore, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, delete, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, useEncryption, arginfo_dao_http_cookie_useencryption, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, isUsingEncryption, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, setExpiration, arginfo_dao_http_cookie_setexpiration, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, getExpiration, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, setPath, arginfo_dao_http_cookie_setpath, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, getPath, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, setDomain, arginfo_dao_http_cookie_setdomain, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, getDomain, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, setSecure, arginfo_dao_http_cookie_setsecure, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, getSecure, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, setHttpOnly, arginfo_dao_http_cookie_sethttponly, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, getHttpOnly, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_ME(Dao_Http_Cookie, __toString, arginfo_empty, ZEND_ACC_PUBLIC)
	PHP_FE_END
};

/**
 * Dao\Http\Cookie initializer
 */
DAO_INIT_CLASS(Dao_Http_Cookie){

	DAO_REGISTER_CLASS_EX(Dao\\Http, Cookie, http_cookie, dao_di_injectable_ce, dao_http_cookie_method_entry, 0);

	zend_declare_property_bool(dao_http_cookie_ce, SL("_readed"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_http_cookie_ce, SL("_restored"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_http_cookie_ce, SL("_useEncryption"), 0, ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_http_cookie_ce, SL("_filter"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_http_cookie_ce, SL("_name"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_http_cookie_ce, SL("_value"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_http_cookie_ce, SL("_expire"), ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_http_cookie_ce, SL("_path"), "/", ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_http_cookie_ce, SL("_domain"), ZEND_ACC_PROTECTED);
	zend_declare_property_null(dao_http_cookie_ce, SL("_secure"), ZEND_ACC_PROTECTED);
	zend_declare_property_bool(dao_http_cookie_ce, SL("_httpOnly"), 1, ZEND_ACC_PROTECTED);
	zend_declare_property_string(dao_http_cookie_ce, SL("_samesite"), "Lax", ZEND_ACC_PROTECTED);

	return SUCCESS;
}

/**
 * Dao\Http\Cookie constructor
 *
 * @param string $name
 * @param mixed $value
 * @param int $expire
 * @param string $path
 * @param boolean $secure
 * @param string $domain
 * @param boolean $httpOnly
 */
PHP_METHOD(Dao_Http_Cookie, __construct){

	zval *name, *value = NULL, *expire = NULL, *path = NULL, *secure = NULL, *domain = NULL, *http_only = NULL, *samesite = NULL;

	dao_fetch_params(0, 1, 7, &name, &value, &expire, &path, &secure, &domain, &http_only, &samesite);
	DAO_ENSURE_IS_STRING(name);

	if (!expire) {
		expire = &DAO_GLOBAL(z_zero);
	}

	dao_update_property(getThis(), SL("_name"), name);

	if (value && Z_TYPE_P(value) != IS_NULL) {
		dao_update_property(getThis(), SL("_value"), value);
		dao_update_property_bool(getThis(), SL("_readed"), 1);
	}

	dao_update_property(getThis(), SL("_expire"), expire);

	if (path && Z_TYPE_P(path) != IS_NULL) {
		dao_update_property(getThis(), SL("_path"), path);
	} else {
		zval tmp = {};
		ZVAL_STRINGL(&tmp, "/", 1);
		dao_update_property(getThis(), SL("_path"), &tmp);
		zval_ptr_dtor(&tmp);
	}

	if (secure && Z_TYPE_P(secure) != IS_NULL) {
		dao_update_property(getThis(), SL("_secure"), secure);
	}

	if (domain && Z_TYPE_P(domain) != IS_NULL) {
		dao_update_property(getThis(), SL("_domain"), domain);
	}

	if (http_only && Z_TYPE_P(http_only) != IS_NULL) {
		dao_update_property(getThis(), SL("_httpOnly"), http_only);
	}

	if (samesite && Z_TYPE_P(samesite) != IS_NULL) {
		dao_update_property(getThis(), SL("_samesite"), samesite);
	}
}

/**
 * Sets the cookie's value
 *
 * @param string $value
 * @return Dao\Http\CookieInterface
 */
PHP_METHOD(Dao_Http_Cookie, setValue){

	zval *value;

	dao_fetch_params(0, 1, 0, &value);

	dao_update_property(getThis(), SL("_value"), value);
	dao_update_property_bool(getThis(), SL("_readed"), 1);
	RETURN_THIS();
}

/**
 * Returns the cookie's value
 *
 * @param string|array $filters
 * @param string $defaultValue
 * @return mixed
 */
PHP_METHOD(Dao_Http_Cookie, getValue)
{
	zval *filters = NULL, *default_value = NULL, restored = {}, dependency_injector = {}, readed = {}, name = {}, *_COOKIE, value = {}, encryption = {};
	zval service = {}, crypt = {}, decrypted_value = {}, filter = {};

	dao_fetch_params(1, 0, 2, &filters, &default_value);

	if (!filters) {
		filters = &DAO_GLOBAL(z_null);
	}

	if (!default_value) {
		default_value = &DAO_GLOBAL(z_null);
	}

	dao_read_property(&restored, getThis(), SL("_restored"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&restored)) {
		DAO_MM_CALL_METHOD(NULL, getThis(), "restore");
	}

	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);
	if (Z_TYPE(dependency_injector) != IS_OBJECT) {
		DAO_MM_THROW_EXCEPTION_STR(dao_http_cookie_exception_ce, "A dependency injection object is required to access the 'filter' service");
		return;
	}

	dao_read_property(&readed, getThis(), SL("_readed"), PH_NOISY|PH_READONLY);
	if (DAO_IS_FALSE(&readed)) {
		dao_read_property(&name, getThis(), SL("_name"), PH_NOISY|PH_READONLY);

		_COOKIE = dao_get_global_str(SL("_COOKIE"));
		if (dao_array_isset_fetch(&value, _COOKIE, &name, PH_READONLY)) {
			dao_read_property(&encryption, getThis(), SL("_useEncryption"), PH_NOISY|PH_READONLY);
			if (zend_is_true(&encryption) && DAO_IS_NOT_EMPTY(&value)) {
				ZVAL_STR(&service, IS(crypt));

				DAO_MM_CALL_METHOD(&crypt, &dependency_injector, "getshared", &service);
				DAO_MM_ADD_ENTRY(&crypt);
				DAO_MM_VERIFY_INTERFACE(&crypt, dao_cryptinterface_ce);

				/**
				 * Decrypt the value also decoding it with base64
				 */
				DAO_MM_CALL_METHOD(&decrypted_value, &crypt, "decryptbase64", &value);
				DAO_MM_ADD_ENTRY(&decrypted_value);
			} else {
				ZVAL_COPY_VALUE(&decrypted_value, &value);
			}

			/**
			 * Update the decrypted value
			 */
			dao_update_property(getThis(), SL("_value"), &decrypted_value);
			if (Z_TYPE_P(filters) != IS_NULL) {
				dao_read_property(&filter, getThis(), SL("_filter"), PH_READONLY);
				if (Z_TYPE(filter) != IS_OBJECT) {
					ZVAL_STR(&service, IS(filter));

					DAO_MM_CALL_METHOD(&filter, &dependency_injector, "getshared", &service);
					DAO_MM_ADD_ENTRY(&filter);
					DAO_MM_VERIFY_INTERFACE(&filter, dao_filterinterface_ce);
					dao_update_property(getThis(), SL("_filter"), &filter);
				}

				DAO_MM_RETURN_CALL_METHOD(&filter, "sanitize", &decrypted_value, filters);
				RETURN_MM();
			}

			/**
			 * Return the value without filtering
			 */
			RETURN_MM_CTOR(&decrypted_value);
			return;
		}
		RETURN_MM_CTOR(default_value);
	}

	dao_read_property(&value, getThis(), SL("_value"), PH_READONLY);

	RETURN_MM_CTOR(&value);
}

/**
 * Sends the cookie to the HTTP client
 * Stores the cookie definition in session
 *
 * @return Dao\Http\Cookie
 */
PHP_METHOD(Dao_Http_Cookie, send){

	zval name = {}, value = {}, expire = {}, domain = {}, path = {}, secure = {}, http_only = {}, samesite = {}, dependency_injector = {};
	zval service = {}, has_session = {}, definition = {}, session = {}, key = {}, encryption = {}, crypt = {}, encrypt_value = {};

	DAO_MM_INIT();

	dao_read_property(&name, getThis(), SL("_name"), PH_NOISY|PH_READONLY);
	dao_read_property(&value, getThis(), SL("_value"), PH_NOISY|PH_READONLY);
	dao_read_property(&expire, getThis(), SL("_expire"), PH_NOISY|PH_READONLY);
	dao_read_property(&domain, getThis(), SL("_domain"), PH_NOISY|PH_READONLY);
	dao_read_property(&path, getThis(), SL("_path"), PH_NOISY|PH_READONLY);
	dao_read_property(&secure, getThis(), SL("_secure"), PH_NOISY|PH_READONLY);
	dao_read_property(&http_only, getThis(), SL("_httpOnly"), PH_NOISY|PH_READONLY);
	dao_read_property(&samesite, getThis(), SL("_samesite"), PH_NOISY|PH_READONLY);
	DAO_MM_CALL_METHOD(&dependency_injector, getThis(), "getdi");
	DAO_MM_ADD_ENTRY(&dependency_injector);

	if (Z_TYPE(dependency_injector) == IS_OBJECT) {
		ZVAL_STR(&service, IS(session));

		DAO_MM_CALL_METHOD(&has_session, &dependency_injector, "has", &service);
		DAO_MM_ADD_ENTRY(&has_session);
		if (zend_is_true(&has_session)) {
			array_init(&definition);
			DAO_MM_ADD_ENTRY(&has_session);
			if (!DAO_IS_LONG(&expire, 0)) {
				dao_array_update_str(&definition, SL("expire"), &expire, PH_COPY);
			}

			if (DAO_IS_NOT_EMPTY(&path)) {
				dao_array_update_str(&definition, SL("path"), &path, PH_COPY);
			}

			if (DAO_IS_NOT_EMPTY(&domain)) {
				dao_array_update_string(&definition, IS(domain), &domain, PH_COPY);
			}

			if (DAO_IS_NOT_EMPTY(&secure)) {
				dao_array_update_str(&definition, SL("secure"), &secure, PH_COPY);
			}

			if (DAO_IS_NOT_EMPTY(&http_only)) {
				dao_array_update_str(&definition, SL("httpOnly"), &http_only, PH_COPY);
			}

			/**
			 * The definition is stored in session
			 */
			if (dao_fast_count_ev(&definition)) {
				DAO_MM_CALL_METHOD(&session, &dependency_injector, "getshared", &service);
				DAO_MM_ADD_ENTRY(&session);

				if (Z_TYPE(session) != IS_NULL) {
					DAO_MM_VERIFY_INTERFACE(&session, dao_session_adapterinterface_ce);

					DAO_CONCAT_SV(&key, "_PHCOOKIE_", &name);
					DAO_MM_ADD_ENTRY(&key);
					DAO_MM_CALL_METHOD(NULL, &session, "set", &key, &definition);
				}
			}
		}
	}

	dao_read_property(&encryption, getThis(), SL("_useEncryption"), PH_READONLY);
	if (zend_is_true(&encryption) && DAO_IS_NOT_EMPTY(&value)) {
		if (Z_TYPE(dependency_injector) != IS_OBJECT) {
			DAO_MM_THROW_EXCEPTION_STR(dao_http_cookie_exception_ce, "A dependency injection object is required to access the 'filter' service");
			return;
		}

		ZVAL_STR(&service, IS(crypt));

		DAO_MM_CALL_METHOD(&crypt, &dependency_injector, "getshared", &service);
		DAO_MM_ADD_ENTRY(&crypt);
		DAO_MM_VERIFY_INTERFACE(&crypt, dao_cryptinterface_ce);

		/**
		 * Encrypt the value also coding it with base64
		 */
		DAO_MM_CALL_METHOD(&encrypt_value, &crypt, "encryptbase64", &value);
		DAO_MM_ADD_ENTRY(&encrypt_value);
	} else {
		ZVAL_COPY_VALUE(&encrypt_value, &value);
	}

	/**
	 * Sets the cookie using the standard 'setcookie' function
	 */
	convert_to_string_ex(&name);
	convert_to_long_ex(&expire);
	convert_to_string_ex(&domain);
	convert_to_string_ex(&path);
	convert_to_long_ex(&secure);
	convert_to_long_ex(&http_only);
	convert_to_string_ex(&encrypt_value);
#if PHP_VERSION_ID >= 70300
	php_setcookie(Z_STR(name), Z_STR(encrypt_value), Z_LVAL(expire), Z_STR(path), Z_STR(domain), Z_LVAL(secure), Z_LVAL(http_only), Z_STR(samesite), 1);
#else
	php_setcookie(Z_STR(name), Z_STR(encrypt_value), Z_LVAL(expire), Z_STR(path), Z_STR(domain), Z_LVAL(secure), 1, Z_LVAL(http_only));
#endif

	RETURN_MM_THIS();
}

/**
 * Reads the cookie-related info from the SESSION to restore the cookie as it was set
 * This method is automatically called internally so normally you don't need to call it
 *
 * @return Dao\Http\Cookie
 */
PHP_METHOD(Dao_Http_Cookie, restore)
{
	zval restored = {}, dependency_injector = {}, service = {}, session = {}, name = {}, key = {}, definition = {}, expire = {};
	zval domain = {}, path = {}, secure = {}, http_only = {};

	DAO_MM_INIT();
	dao_read_property(&restored, getThis(), SL("_restored"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&restored)) {
		DAO_CALL_METHOD(&dependency_injector, getThis(), "getdi");
		DAO_MM_ADD_ENTRY(&dependency_injector);
		if (Z_TYPE(dependency_injector) == IS_OBJECT) {
			ZVAL_STR(&service, IS(session));

			DAO_MM_CALL_METHOD(&session, &dependency_injector, "getshared", &service);
			DAO_MM_ADD_ENTRY(&session);
			DAO_MM_VERIFY_INTERFACE(&session, dao_session_adapterinterface_ce);

			dao_read_property(&name, getThis(), SL("_name"), PH_READONLY);

			DAO_CONCAT_SV(&key, "_PHCOOKIE_", &name);
			DAO_MM_ADD_ENTRY(&key);
			DAO_MM_CALL_METHOD(&definition, &session, "get", &key);
			DAO_MM_ADD_ENTRY(&definition);
			if (Z_TYPE(definition) == IS_ARRAY) {
				if (dao_array_isset_fetch_str(&expire, &definition, SL("expire"), PH_READONLY)) {
					dao_update_property(getThis(), SL("_expire"), &expire);
				}
				if (dao_array_isset_fetch_str(&domain, &definition, SL("domain"), PH_READONLY)) {
					dao_update_property(getThis(), SL("_domain"), &domain);
				}

				if (dao_array_isset_fetch_str(&path, &definition, SL("path"), PH_READONLY)) {
					dao_update_property(getThis(), SL("_path"), &path);
				}

				if (dao_array_isset_fetch_str(&secure, &definition, SL("secure"), PH_READONLY)) {
					dao_update_property(getThis(), SL("_secure"), &secure);
				}

				if (dao_array_isset_fetch_str(&http_only, &definition, SL("httpOnly"), PH_READONLY)) {
					dao_update_property(getThis(), SL("_httpOnly"), &http_only);
				}
			}
		}

		dao_update_property_bool(getThis(), SL("_restored"), 1);
	}

	RETURN_MM_THIS();
}

/**
 * Deletes the cookie by setting an expire time in the past
 *
 */
PHP_METHOD(Dao_Http_Cookie, delete)
{
	zval name = {}, domain = {}, path = {}, secure = {}, http_only = {}, samesite = {}, service = {}, session = {}, key = {};

	DAO_MM_INIT();

	dao_read_property(&name, getThis(), SL("_name"), PH_NOISY|PH_READONLY);
	dao_read_property(&domain, getThis(), SL("_domain"), PH_NOISY|PH_READONLY);
	dao_read_property(&path, getThis(), SL("_path"), PH_NOISY|PH_READONLY);
	dao_read_property(&secure, getThis(), SL("_secure"), PH_NOISY|PH_READONLY);
	dao_read_property(&http_only, getThis(), SL("_httpOnly"), PH_NOISY|PH_READONLY);
	dao_read_property(&samesite, getThis(), SL("_samesite"), PH_NOISY|PH_READONLY);

	ZVAL_STR(&service, IS(session));

	DAO_MM_CALL_METHOD(&session, getThis(), "getresolveservice", &service);
	DAO_MM_ADD_ENTRY(&session);
	DAO_MM_VERIFY_INTERFACE(&session, dao_session_adapterinterface_ce);

	DAO_CONCAT_SV(&key, "_PHCOOKIE_", &name);
	DAO_MM_ADD_ENTRY(&key);
	DAO_MM_CALL_METHOD(NULL, &session, "remove", &key);

	dao_update_property_null(getThis(), SL("_value"));

	convert_to_string_ex(&name);
	convert_to_string_ex(&path);
	convert_to_string_ex(&domain);
	convert_to_long_ex(&secure);
	convert_to_long_ex(&http_only);
#if PHP_VERSION_ID >= 70300
	php_setcookie(Z_STR(name), NULL, time(NULL) - 691200, Z_STR(path), Z_STR(domain), Z_LVAL(secure), Z_LVAL(http_only), Z_STR(samesite), 1);
#else
	php_setcookie(Z_STR(name), NULL, time(NULL) - 691200, Z_STR(path), Z_STR(domain), Z_LVAL(secure), 1, Z_LVAL(http_only));
#endif
	RETURN_MM();
}

/**
 * Sets if the cookie must be encrypted/decrypted automatically
 *
 * @param boolean $useEncryption
 * @return Dao\Http\Cookie
 */
PHP_METHOD(Dao_Http_Cookie, useEncryption){

	zval *use_encryption;

	dao_fetch_params(0, 1, 0, &use_encryption);

	dao_update_property(getThis(), SL("_useEncryption"), use_encryption);
	RETURN_THIS();
}

/**
 * Check if the cookie is using implicit encryption
 *
 * @return boolean
 */
PHP_METHOD(Dao_Http_Cookie, isUsingEncryption){


	RETURN_MEMBER(getThis(), "_useEncryption");
}

/**
 * Sets the cookie's expiration time
 *
 * @param int $expire
 * @return Dao\Http\Cookie
 */
PHP_METHOD(Dao_Http_Cookie, setExpiration){

	zval *expire, restored = {};

	dao_fetch_params(0, 1, 0, &expire);

	dao_read_property(&restored, getThis(), SL("_restored"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&restored)) {
		DAO_CALL_METHOD(NULL, getThis(), "restore");
	}

	dao_update_property(getThis(), SL("_expire"), expire);

	RETURN_THIS();
}

/**
 * Returns the current expiration time
 *
 * @return string
 */
PHP_METHOD(Dao_Http_Cookie, getExpiration){

	zval restored = {}, expire = {};

	dao_read_property(&restored, getThis(), SL("_restored"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&restored)) {
		DAO_CALL_METHOD(NULL, getThis(), "restore");
	}

	dao_read_property(&expire, getThis(), SL("_expire"), PH_NOISY|PH_READONLY);

	RETURN_CTOR(&expire);
}

/**
 * Sets the cookie's expiration time
 *
 * @param string $path
 * @return Dao\Http\Cookie
 */
PHP_METHOD(Dao_Http_Cookie, setPath){

	zval *path, restored = {};

	dao_fetch_params(0, 1, 0, &path);

	dao_read_property(&restored, getThis(), SL("_restored"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&restored)) {
		DAO_CALL_METHOD(NULL, getThis(), "restore");
	}

	dao_update_property(getThis(), SL("_path"), path);

	RETURN_THIS();
}

/**
 * Returns the current cookie's path
 *
 * @return string
 */
PHP_METHOD(Dao_Http_Cookie, getPath){

	zval restored = {}, path = {};

	dao_read_property(&restored, getThis(), SL("_restored"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&restored)) {
		DAO_CALL_METHOD(NULL, getThis(), "restore");
	}

	dao_read_property(&path, getThis(), SL("_path"), PH_NOISY|PH_READONLY);

	RETURN_CTOR(&path);
}

/**
 * Sets the domain that the cookie is available to
 *
 * @param string $domain
 * @return Dao\Http\Cookie
 */
PHP_METHOD(Dao_Http_Cookie, setDomain){

	zval *domain, restored = {};

	dao_fetch_params(0, 1, 0, &domain);

	dao_read_property(&restored, getThis(), SL("_restored"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&restored)) {
		DAO_CALL_METHOD(NULL, getThis(), "restore");
	}

	dao_update_property(getThis(), SL("_domain"), domain);

	RETURN_THIS();
}

/**
 * Returns the domain that the cookie is available to
 *
 * @return string
 */
PHP_METHOD(Dao_Http_Cookie, getDomain){

	zval restored = {}, domain = {};

	dao_read_property(&restored, getThis(), SL("_restored"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&restored)) {
		DAO_CALL_METHOD(NULL, getThis(), "restore");
	}

	dao_read_property(&domain, getThis(), SL("_domain"), PH_NOISY|PH_READONLY);

	RETURN_CTOR(&domain);
}

/**
 * Sets if the cookie must only be sent when the connection is secure (HTTPS)
 *
 * @param boolean $secure
 * @return Dao\Http\Cookie
 */
PHP_METHOD(Dao_Http_Cookie, setSecure){

	zval *secure, restored = {};

	dao_fetch_params(0, 1, 0, &secure);

	dao_read_property(&restored, getThis(), SL("_restored"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&restored)) {
		DAO_CALL_METHOD(NULL, getThis(), "restore");
	}

	dao_update_property(getThis(), SL("_secure"), secure);

	RETURN_THIS();
}

/**
 * Returns whether the cookie must only be sent when the connection is secure (HTTPS)
 *
 * @return boolean
 */
PHP_METHOD(Dao_Http_Cookie, getSecure){

	zval restored = {}, secure = {};

	dao_read_property(&restored, getThis(), SL("_restored"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&restored)) {
		DAO_CALL_METHOD(NULL, getThis(), "restore");
	}

	dao_read_property(&secure, getThis(), SL("_secure"), PH_NOISY|PH_READONLY);

	RETURN_CTOR(&secure);
}

/**
 * Sets if the cookie is accessible only through the HTTP protocol
 *
 * @param boolean $httpOnly
 * @return Dao\Http\Cookie
 */
PHP_METHOD(Dao_Http_Cookie, setHttpOnly){

	zval *http_only, restored = {};

	dao_fetch_params(0, 1, 0, &http_only);

	dao_read_property(&restored, getThis(), SL("_restored"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&restored)) {
		DAO_CALL_METHOD(NULL, getThis(), "restore");
	}

	dao_update_property(getThis(), SL("_httpOnly"), http_only);

	RETURN_THIS();
}

/**
 * Returns if the cookie is accessible only through the HTTP protocol
 *
 * @return boolean
 */
PHP_METHOD(Dao_Http_Cookie, getHttpOnly){

	zval restored = {}, http_only = {};

	dao_read_property(&restored, getThis(), SL("_restored"), PH_NOISY|PH_READONLY);
	if (!zend_is_true(&restored)) {
		DAO_CALL_METHOD(NULL, getThis(), "restore");
	}

	dao_read_property(&http_only, getThis(), SL("_httpOnly"), PH_NOISY|PH_READONLY);

	RETURN_CTOR(&http_only);
}

/**
 * Magic __toString method converts the cookie's value to string
 *
 * @return mixed
 */
PHP_METHOD(Dao_Http_Cookie, __toString){

	zval value = {}, exception = {}, *m;

	dao_read_property(&value, getThis(), SL("_value"), PH_NOISY|PH_READONLY);
	if (Z_TYPE(value) == IS_NULL) {
		if (FAILURE == dao_call_method(return_value, getThis(), "getvalue", 0, NULL)) {
			if (EG(exception)) {

#if PHP_VERSION_ID >= 80000
				ZVAL_OBJ(&exception, zend_objects_clone_obj(EG(exception)));
				m = zend_read_property(Z_OBJCE(exception), EG(exception), SL("message"), 1, NULL);
#else
				zval e = {};
				ZVAL_OBJ(&e, EG(exception));
				ZVAL_OBJ(&exception, zend_objects_clone_obj(&e));
				m = zend_read_property(Z_OBJCE(exception), &exception, SL("message"), 1, NULL);
#endif

				Z_TRY_ADDREF_P(m);
				if (Z_TYPE_P(m) != IS_STRING) {
					convert_to_string_ex(m);
				}

				zend_clear_exception();
				zend_error(E_ERROR, "%s", Z_STRVAL_P(m));
			}
		}

		convert_to_string(return_value);
		return;
	}

	RETURN_CTOR(&value);
}
