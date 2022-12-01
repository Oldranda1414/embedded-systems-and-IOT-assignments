package main_package;


public class JavaOutputProgram {

	public static void main(String[] args) throws Exception {
		CommChannel channel = new SerialCommChannel(args[0],9600);	
		System.out.println("welcome to the smart bridge interface");
		while (true){
			
			if(channel.isMsgAvailable()) {
				
				String msg = channel.receiveMsg();
				String[] msgArray = msg.split("/");
				if(msgArray.length == 3) {
					System.out.println("the current state of the monitoring system is " + msgArray[0]);
					System.out.println("the current state of the smart lighting system is " + msgArray[1]);
					System.out.println("the current water level is " + msgArray[2] + "m");
					System.out.print("\n\n\n");
				}
				
				
			}
			
			
		}
		
		
	}

}
