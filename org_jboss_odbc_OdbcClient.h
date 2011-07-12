/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include <sql.h>
#include <sqlext.h>
/* Header for class org_jboss_odbc_OdbcClient */

#ifndef _Included_org_jboss_odbc_OdbcClient
#define _Included_org_jboss_odbc_OdbcClient
#ifdef __cplusplus
extern "C" {
#endif

SQLHENV env[1024];
SQLHDBC dbc[1024];
SQLHSTMT stmt[1024];

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    connect
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_jboss_odbc_OdbcClient_connect
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    execute
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_jboss_odbc_OdbcClient_execute
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    getNumCols
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_jboss_odbc_OdbcClient_getNumCols
  (JNIEnv *, jobject);

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    getColMetadata
 * Signature: (I)Lorg/jboss/odbc/OdbcClient/ColumnMetadata;
 */
JNIEXPORT jobject JNICALL Java_org_jboss_odbc_OdbcClient_getColMetadata
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    fetch
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_jboss_odbc_OdbcClient_fetch
  (JNIEnv *, jobject);

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    getColData
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_jboss_odbc_OdbcClient_getColData
  (JNIEnv *, jobject, jint);

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    freeStatement
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_jboss_odbc_OdbcClient_freeStatement
  (JNIEnv *, jobject);

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_jboss_odbc_OdbcClient_close
  (JNIEnv *, jobject);

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    disconnect
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_jboss_odbc_OdbcClient_disconnect
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
