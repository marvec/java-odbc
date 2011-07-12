#include <stdio.h>
#include <sql.h>
#include <sqlext.h>
#include "org_jboss_odbc_OdbcClient.h"
#include "util.h"

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    connect
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_jboss_odbc_OdbcClient_connect(JNIEnv *jnienv, jobject obj, jstring conn) {
    SQLRETURN ret;
    jbyte *str;
    int h = getStaticHandle(jnienv, obj);  
    char error[10240];  

    jclass cls = (*jnienv)->GetObjectClass(jnienv, obj);

    str = (*jnienv)->GetStringUTFChars(jnienv, conn, NULL);
    
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &(env[h]));
    SQLSetEnvAttr(env[h], SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, env[h], &(dbc[h]));
    ret = SQLDriverConnect(dbc[h], NULL, str, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);

    if (!SQL_SUCCEEDED(ret)) {
        extractError(dbc[h], SQL_HANDLE_DBC, error, sizeof(error));        
        throwIOException(jnienv, "SQLDriverConnect failed with return value %d:\n%s", ret, error);
    }
    
    (*jnienv)->ReleaseStringUTFChars(jnienv, conn, str);
}

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    execute
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_jboss_odbc_OdbcClient_execute(JNIEnv *jnienv, jobject obj, jstring query) {
    SQLRETURN ret;
    char error[10240];
    jbyte *str;
    int h = getStaticHandle(jnienv, obj);

    str = (*jnienv)->GetStringUTFChars(jnienv, query, NULL);
    
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc[h], &(stmt[h]));
    if (!SQL_SUCCEEDED(ret)) {
        extractError(dbc[h], SQL_HANDLE_DBC, error, sizeof(error));        
        throwIOException(jnienv, "SQLAllocHandle failed with return value %d:\n%s", ret, error);        
    }
    
    ret = SQLExecDirect(stmt[h], str, SQL_NTS);
    (*jnienv)->ReleaseStringUTFChars(jnienv, query, str);    

    if (!SQL_SUCCEEDED(ret)) {
        extractError(stmt[h], SQL_HANDLE_STMT, error, sizeof(error));        
        throwIOException(jnienv, "SQLExecDirect failed with return value %d:\n%s", ret, error);        
    }
    
}

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    getNumCols
 * Signature: ()J
 */
JNIEXPORT jint JNICALL Java_org_jboss_odbc_OdbcClient_getNumCols(JNIEnv *jnienv, jobject obj) {
    SQLSMALLINT columns;
    int h = getStaticHandle(jnienv, obj);
    
    SQLNumResultCols(stmt[h], &columns);
    
    return (jint) columns;
}

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    fetch
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_jboss_odbc_OdbcClient_fetch(JNIEnv *jnienv, jobject obj) {
    int h = getStaticHandle(jnienv, obj);

    return (jboolean) SQL_SUCCEEDED(SQLFetch(stmt[h]));
}

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    getColData
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_jboss_odbc_OdbcClient_getColData(JNIEnv *jnienv, jobject obj, jint col) {
    int h = getStaticHandle(jnienv, obj);
    SQLINTEGER indicator;
    SQLRETURN ret;
    char buf[5120];
    char error[10240];
       
    ret = SQLGetData(stmt[h], (SQLUSMALLINT) col, SQL_C_CHAR, buf, sizeof(buf), &indicator);
    
    if (SQL_SUCCEEDED(ret)) {
        if (indicator == SQL_NULL_DATA) strcpy(buf, "NULL");
    } else {
        extractError(stmt[h], SQL_HANDLE_STMT, error, sizeof(error));        
        throwIOException(jnienv, "SQLGetData failed with return value %d:\n%s", ret, error);
    }
        
    return (*jnienv)->NewStringUTF(jnienv, buf);
}

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    getColMetadata
 * Signature: (ILorg/jboss/odbc/OdbcClient/ColumnMetadata;)V
 */
JNIEXPORT jobject JNICALL Java_org_jboss_odbc_OdbcClient_getColMetadata(JNIEnv *jnienv, jobject obj, jint col) {
    int h = getStaticHandle(jnienv, obj);
    SQLSMALLINT nameLength, dataType, decimalDigits, nullable;
    SQLULEN colSize;
    jstring jstr;
    char name[512];

    SQLDescribeCol(stmt[h], (SQLUSMALLINT) col, name, sizeof(name), &nameLength, &dataType, &colSize, &decimalDigits, &nullable);
    
    jstr = (*jnienv)->NewStringUTF(jnienv, name);
    
    jclass clazz;
    jmethodID cid;
    jobject meta;

    clazz = (*jnienv)->FindClass(jnienv, "org/jboss/odbc/OdbcClient$ColumnMetadata");
    cid = (*jnienv)->GetMethodID(jnienv, clazz, "<init>", "(Ljava/lang/String;IJIZ)V");
    meta = (*jnienv)->NewObject(jnienv, clazz, cid, jstr, dataType, colSize, decimalDigits, (jboolean) (nullable == SQL_NULLABLE));  

    return meta;
}


/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    freeStatement
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_jboss_odbc_OdbcClient_freeStatement(JNIEnv *jnienv, jobject obj) {
    int h = getStaticHandle(jnienv, obj);

    SQLFreeHandle(SQL_HANDLE_STMT, stmt[h]);
}

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_jboss_odbc_OdbcClient_close(JNIEnv *jnienv, jobject obj) {
    int h = getStaticHandle(jnienv, obj);

    SQLFreeHandle(SQL_HANDLE_ENV, env[h]);   
}

/*
 * Class:     org_jboss_odbc_OdbcClient
 * Method:    disconnect
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_jboss_odbc_OdbcClient_disconnect(JNIEnv *jnienv, jobject obj) {
    int h = getStaticHandle(jnienv, obj);

    SQLDisconnect(dbc[h]);
    SQLFreeHandle(SQL_HANDLE_DBC, dbc[h]);
}

