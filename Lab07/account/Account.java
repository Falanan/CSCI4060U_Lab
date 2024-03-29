package account;

//import java.lang.*;

// public class Account {
//     double amount;
//     String  name;

//     //constructor
//     public Account(String nm,double amnt ) {
//         amount=amnt;
//         name=nm;
//     }
//     //functions
//     synchronized  void depsite(double money){
//         amount+=money;
//     }

//     synchronized  void withdraw(double money){
//         amount-=money;
//     }

//     synchronized  void transfer(Account ac,double mn){
//         amount-=mn;
//         ac.amount+=mn;
//     }

//     synchronized void print(){
//         System.out.println(name + "--"+amount);
//     }

// }//end of class Acount



public class Account {
    double amount;
    String  name;
    private final Object lock = new Object();

    //constructor
    public Account(String nm,double amnt) {
        amount=amnt;
        name=nm;
    }
  //functions
  synchronized void depsite(double money){
        synchronized(lock) {
            amount += money;
        }
    }

    synchronized void withdraw(double money){
        synchronized(lock){
            amount-=money;
        }
    }

    synchronized void transfer(Account ac,double mn){
        synchronized(lock){
            amount-=mn;
            ac.amount+=mn;
        }
    }

    synchronized void print(){
        synchronized(lock){
            System.out.println(name + "--"+amount);
        }
    }

}//end of class Acount



