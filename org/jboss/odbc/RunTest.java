/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package org.jboss.odbc;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author pmacik
 */
public class RunTest {
   private static ExecutorService executor;

   public static void main(String[] args) {
      try {
         executor = Executors.newFixedThreadPool(Integer.valueOf(args[3]));
         System.loadLibrary(args[0]);
         int count = Integer.valueOf(args[4]);
         //      String s = System.console().readLine();
         long pre = System.currentTimeMillis();
         for (int i = 0; i < count; i++) {
            //         System.out.println("Executing iteration " + (i + 1) + "/" + count);
            executor.execute(new ODBCClientExecutor(args[1], args[2]));
         }
         executor.shutdown();
         executor.awaitTermination(1, TimeUnit.HOURS);
         long post = System.currentTimeMillis();
         long time = post - pre;
         Logger.getLogger(RunTest.class.getName()).log(Level.INFO, "{0} iteration was performed in {1} ms ({2} it/s)", new Object[]{count, time, 1000f * count / time});
      } catch (InterruptedException ex) {
         Logger.getLogger(RunTest.class.getName()).log(Level.SEVERE, null, ex);
      }
   }
}
