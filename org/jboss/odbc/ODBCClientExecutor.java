/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.jboss.odbc;

import java.io.IOException;

/**
 *
 * @author pmacik
 */
public class ODBCClientExecutor implements Runnable {
   private String odbcConnection = "";
   private String query = "";

   public ODBCClientExecutor(String odbcConnection, String query) {
      this.odbcConnection = odbcConnection;
      this.query = query;
   }

   @Override
   public void run() {
//      log("Running...");
      try {
         long pre = System.currentTimeMillis();
         OdbcClient c = new OdbcClient();
//         log("Connecting using: " + odbcConnection + "...");
         c.connect(odbcConnection);
//         log("Executing query: " + query + "...");
         c.execute(query);
//         log("Freeing statement...");
         c.freeStatement();
//         log("Disconnecting...");
         c.disconnect();
         c.finish();
         long post = System.currentTimeMillis();
         log((post - pre) + " ms");
      } catch (IOException ioe) {
         ioe.printStackTrace();
      }
   }

   private void log(String msg) {
      System.out.println("[" + getClass().getName() + " : " + Thread.currentThread().getName() + "] " + msg);
   }
}
