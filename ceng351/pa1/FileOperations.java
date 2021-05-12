package ceng.ceng351.cengtubedb;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;


public class FileOperations {
	
	
	public static FileWriter createFileWriter( String path) throws IOException {

		File f = new File( path);

		FileWriter fileWriter = null;

		if( f.isDirectory() && !f.exists())
			f.mkdirs();
		else if( !f.isDirectory() && !f.getParentFile().exists())
			f.getParentFile().mkdirs();

		if( !f.isDirectory() && f.exists())
			f.delete();

		fileWriter = new FileWriter( f, false);

		return fileWriter;
	}
	
	
	public static User[] readUserFile(String pathToFile){
		
		FileReader fileReader = null;
		BufferedReader bufferedReader = null; 
		
		String strLine;
		
		List<User> userList = new ArrayList<User>();
		User[] userArray = null;
		
		int indexofFirstTab;
		int indexofSecondTab;
		int indexofThirdTab;
		int indexofFourthTab;
		
		
		Integer userID = null;
		String userName = null;
		String email = null;
		String password = null;
		String status = null;
		
		User user = null;
		
		try {
			
			fileReader = new FileReader(pathToFile);
			bufferedReader = new BufferedReader(fileReader);

			while((strLine = bufferedReader.readLine())!=null){
				//parse strLine
				indexofFirstTab = strLine.indexOf('\t');
				indexofSecondTab = strLine.indexOf('\t',indexofFirstTab+1);
				indexofThirdTab = strLine.indexOf('\t',indexofSecondTab+1);
				indexofFourthTab = strLine.indexOf('\t',indexofThirdTab+1);

				if (indexofFirstTab!=-1 && indexofSecondTab!= -1 && indexofThirdTab!=-1){
					
					userID = Integer.parseInt(strLine.substring(0,indexofFirstTab));
					userName = strLine.substring(indexofFirstTab+1,indexofSecondTab);
					email = strLine.substring(indexofSecondTab+1,indexofThirdTab);
					password = strLine.substring(indexofThirdTab+1,indexofFourthTab);
					status = strLine.substring(indexofFourthTab+1);
					
				}else{
					System.out.println("There is a problem in User File Reading phase");
				}
					
				user = new User(userID,userName,email,password,status);
				userList.add(user);
				
			}
			
			bufferedReader.close();
		
			userArray = new User[userList.size()];
			userList.toArray(userArray);
		
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return userArray;
	}
		
	
	public static Video[] readVideoFile(String pathToFile){
		
		FileReader fileReader = null;
		BufferedReader bufferedReader = null; 
		
		String strLine;
		
		List<Video> videoList = new ArrayList<Video>();
		Video[] videoArray = null;
		
		int indexofFirstTab;
		int indexofSecondTab;
		int indexofThirdTab;
		int indexofFourthTab;
		int indexofFifthTab;
		
		Integer videoID = null;
		Integer userID = null;
		String videoTitle = null;
		Integer likeCount = null;
		Integer dislikeCount = null;
		String datePublished = null;
		
		Video video = null;
		
		try {
			
			fileReader = new FileReader(pathToFile);
			bufferedReader = new BufferedReader(fileReader);

			
			while((strLine = bufferedReader.readLine())!=null){
				
				//parse strLine
				indexofFirstTab = strLine.indexOf('\t');
				indexofSecondTab = strLine.indexOf('\t',indexofFirstTab+1);
				indexofThirdTab = strLine.indexOf('\t',indexofSecondTab+1);
				indexofFourthTab = strLine.indexOf('\t',indexofThirdTab+1);
				indexofFifthTab = strLine.indexOf('\t',indexofFourthTab+1);
				
				if (indexofFirstTab!=-1 && indexofSecondTab!= -1 && indexofThirdTab!=-1 && indexofFourthTab!=-1 && indexofFifthTab!= -1){

					videoID = Integer.parseInt(strLine.substring(0,indexofFirstTab));
					userID = Integer.parseInt(strLine.substring(indexofFirstTab+1,indexofSecondTab));
					videoTitle = strLine.substring(indexofSecondTab+1,indexofThirdTab);
					likeCount = Integer.parseInt(strLine.substring(indexofThirdTab+1,indexofFourthTab));
					dislikeCount = Integer.parseInt(strLine.substring(indexofFourthTab+1,indexofFifthTab));
					datePublished = strLine.substring(indexofFifthTab+1);
					
				}else{
					System.out.println("There is a problem in Video File Reading phase");
				}
					
				video = new Video(videoID, userID, videoTitle, likeCount, dislikeCount, datePublished);
				videoList.add(video);
				
			}
			
			bufferedReader.close();
		
			videoArray = new Video[videoList.size()];
			videoList.toArray(videoArray);

		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return videoArray;
	}
	
	
	public static Comment[] readCommentFile(String pathToFile){
		
		FileReader fileReader = null;
		BufferedReader bufferedReader = null; 
		
		String strLine;
		
		List<Comment> commentList = new ArrayList<Comment>();
		Comment[] commentArray = null;
		
		int indexofFirstTab;
		int indexofSecondTab;
		int indexofThirdTab;
		int indexofFourthTab;

		
		Integer commentID = null;
		Integer userID = null;
		Integer videoID = null;
		String commentText = null;
		String dateCommented = null;

		Comment comment = null;
		
		try {
			
			fileReader = new FileReader(pathToFile);
			bufferedReader = new BufferedReader(fileReader);
			
			while((strLine = bufferedReader.readLine())!=null){
				
				//parse strLine
				indexofFirstTab = strLine.indexOf('\t');
				indexofSecondTab = strLine.indexOf('\t',indexofFirstTab+1);
				indexofThirdTab = strLine.indexOf('\t',indexofSecondTab+1);
				indexofFourthTab = strLine.indexOf('\t',indexofThirdTab+1);

				if (indexofFirstTab!=-1 && indexofSecondTab!= -1 && indexofThirdTab!=-1 && indexofFourthTab!=-1){
					
					commentID = Integer.parseInt(strLine.substring(0,indexofFirstTab));
					userID = Integer.parseInt(strLine.substring(indexofFirstTab+1,indexofSecondTab));
					videoID = Integer.parseInt(strLine.substring(indexofSecondTab+1,indexofThirdTab));
					commentText = strLine.substring(indexofThirdTab+1,indexofFourthTab);
					dateCommented = strLine.substring(indexofFourthTab+1);
					
				}else{
					System.out.println("There is a problem in Comment File Reading phase");
				}
					
				comment = new Comment(commentID,userID,videoID,commentText,dateCommented);
				commentList.add(comment);
				
			}
			
			bufferedReader.close();
		
			commentArray = new Comment[commentList.size()];
			commentList.toArray(commentArray);
		
			
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return commentArray;
	}

	public static Watch[] readWatchFile(String pathToFile){
		
		FileReader fileReader = null;
		BufferedReader bufferedReader = null; 
		
		String strLine;
		
		List<Watch> watchList = new ArrayList<Watch>();
		Watch[] watchArray = null;
		
		int indexofFirstTab;
		int indexofSecondTab;

		Integer userID = null;
		Integer videoID = null;
		String dateWatched = null;

		Watch watch = null;
		
		try {
			
			fileReader = new FileReader(pathToFile);
			bufferedReader = new BufferedReader(fileReader);
			
			while((strLine = bufferedReader.readLine())!=null){
				
				indexofFirstTab = strLine.indexOf('\t');
				indexofSecondTab = strLine.indexOf('\t',indexofFirstTab+1);
				
				if (indexofFirstTab!=-1 && indexofSecondTab!= -1){

					userID = Integer.parseInt(strLine.substring(0,indexofFirstTab));
					videoID = Integer.parseInt(strLine.substring(indexofFirstTab+1,indexofSecondTab));
					dateWatched = strLine.substring(indexofSecondTab+1);

				}else{
					System.out.println("There is a problem in Watch File Reading phase");
				}
					
				watch = new Watch(userID,videoID,dateWatched);
				watchList.add(watch);
				
			}

			bufferedReader.close();
		
			watchArray = new Watch[watchList.size()];
			watchList.toArray(watchArray);
		
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		return watchArray;
	}


}
