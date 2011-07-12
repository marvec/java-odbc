package org.jboss.odbc;

import java.io.IOException;
import java.util.SortedSet;
import java.util.TreeSet;

public class OdbcClient {
   private static OdbcClient instance = null;
   private static SortedSet<Integer> usedHandles = new TreeSet<Integer>();
   private volatile int handle = -1;
   private static final Object lock = new Object();
   private static final Object handleLock = new Object();

   public native void connect(String connection) throws IOException;

   public native void execute(String statement) throws IOException;

   public native int getNumCols();

   public native ColumnMetadata getColMetadata(int col);

   public native boolean fetch();

   public native String getColData(int col) throws IOException;

   public native void freeStatement();

   public native void disconnect();

   public native void close();

   public OdbcClient() throws IOException {
      synchronized (handleLock) {
         int i = 1;
         while (usedHandles.contains(i) && i <= 1024) {
            i++;
         }
         if (i == 1024) {
            throw new IOException("All handles are currently in use. Try to free some handles by disconnecting from ODBC.");
         }
         usedHandles.add(i);
         handle = i;
      }
   }

   public static OdbcClient getInstance() {
      synchronized (lock) {
         if (instance == null) {
            try {
               instance = new OdbcClient();
            } catch (IOException ioe) {
               ioe.printStackTrace();
            }
         }
      }
      return instance;
   }

   public void finish() {
      close();
//      synchronized (handleLock) {
      usedHandles.remove(handle);
//      }
   }

   public static class ColumnMetadata {
      public String name;
      public int type;
      public long length;
      public int digits;
      public boolean nullable;

      public ColumnMetadata() {
      }

      public ColumnMetadata(String name, int type, long length, int digits, boolean nullable) {
         this.name = name;
         this.type = type;
         this.length = length;
         this.digits = digits;
         this.nullable = nullable;
      }

      public String toString() {
         return name + ": " + type + "(" + length + ")" + "[" + digits + "]" + (nullable ? "*" : "");
      }
   }
//   public static void main(String... args) throws Exception {
//      System.loadLibrary(args[0]);
//      OdbcClient c = new OdbcClient();
//      //c.connect("DRIVER={PostgreSQL64};DATABASE=perf;SERVER=jawa09;PORT=35432;Uid=admin;Pwd=teiid;");
//      //System.out.println("ODBC Connecting: [" + args[1] + "]");
//      c.connect(args[1]);
//      //c.execute("select * from persons where person_id < 1000");
//      //System.out.println("Executing SQL: [" + args[2] + "]");
//      c.execute(args[2]);
//      /*System.out.println("Columns: " + c.getNumCols());
//      for (int i = 1, j = c.getNumCols(); i <= j; i++) {
//      ColumnMetadata meta = c.getColMetadata(i);
//      System.out.println("Column " + i + ": " + meta);
//      }
//      while (c.fetch()) {
//      for (int i = 1, j = c.getNumCols(); i <= j; i++) {
//      System.out.print(c.getColData(i) + (i == j ? "\n" : ", "));
//      }
//      }*/
//      c.freeStatement();
//      c.disconnect();
//   }
}
