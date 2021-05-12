package ceng.ceng351.cengtubedb;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;



public class Evaluation {


	public static void addInputTitle(String title, BufferedWriter bufferedWriter) throws IOException {
		bufferedWriter.write("*** " + title + " ***" + System.getProperty("line.separator"));
	}

	
	private static void printException(SQLException ex) {
		System.out.println(ex.getMessage() + "\n");
	}
	
	public static void printLine(String result, BufferedWriter bufferedWriter) throws IOException {
		bufferedWriter.write(result + System.getProperty("line.separator"));
	}
	
	public static void addDivider(BufferedWriter bufferedWriter) throws IOException {
		bufferedWriter.write( System.getProperty("line.separator")+ "--------------------------------------------------------------" + System.getProperty("line.separator"));
	}


	public static void main(String[] args) {
		int numberOfInsertions = 0;
		int numberOfTablesCreated = 0;
		int numberOfTablesDropped = 0;

		/***********************************************************/
		// TODO While running on your local machine, change cengtubedb Directory accordingly
		String cengTubeDBDirectory = "/Users/oznakn/Documents/METU/ceng351/pa1/src/ceng/ceng351/cengtubedb";
		/***********************************************************/
		
		FileWriter fileWriter = null;
		BufferedWriter bufferedWriter = null;


		
		try {
			
			// Create cengTubeDB object and initialize
			CengTubeDB cengTubeDB = new CengTubeDB();
			cengTubeDB.initialize();


			/***********************************************************/
			/*************Create File Writer starts*********************/
			/***********************************************************/
			fileWriter = FileOperations.createFileWriter( cengTubeDBDirectory + System.getProperty("file.separator") + "output" + System.getProperty("file.separator") + "Output.txt");
			bufferedWriter =  new BufferedWriter(fileWriter);
			/***********************************************************/
			/*************Create File Writer ends***********************/
			/***********************************************************/
		
			
			/***********************************************************/
			/*******************Create tables starts********************/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("Create tables",bufferedWriter);
			numberOfTablesCreated = 0;
			
			// Create Tables
			try {
				numberOfTablesCreated = cengTubeDB.createTables();
				
				// Check if tables are created
				printLine("Created " + numberOfTablesCreated + " tables.", bufferedWriter);

			} catch(Exception e) {
				e.printStackTrace();
			}
			
			addDivider(bufferedWriter);
			/***********************************************************/
			/*******************Create tables ends**********************/
			/***********************************************************/
			
			
			/***********************************************************/
			/*******************Insert INTO User starts*****************/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("Insert into User",bufferedWriter);
			//insert User
			numberOfInsertions = 0;
			User[] users = FileOperations.readUserFile(
			cengTubeDBDirectory + System.getProperty("file.separator") +
					"data" + System.getProperty("file.separator") +
					"Test_UserFile.txt");
			
			numberOfInsertions = cengTubeDB.insertUser(users);
			printLine( numberOfInsertions + " users are inserted.",bufferedWriter);
			addDivider(bufferedWriter);
			/***********************************************************/
			/*******************Insert INTO User ends*******************/
			/***********************************************************/
			
			
			/***********************************************************/
			/*******************Insert INTO Video starts**********/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("Insert into Video",bufferedWriter);
			//insert Video
			numberOfInsertions = 0;
			Video[] videos = FileOperations.readVideoFile(
			cengTubeDBDirectory + System.getProperty("file.separator") +
					"data" + System.getProperty("file.separator") +
					"Test_VideoFile.txt");
			
			numberOfInsertions = cengTubeDB.insertVideo(videos);
			printLine( numberOfInsertions + " video are inserted.", bufferedWriter);
			addDivider(bufferedWriter);
			/***********************************************************/
			/*******************Insert INTO Video ends************/
			/***********************************************************/
	
			
			/***********************************************************/
			/*******************Insert INTO Comment starts*************/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("Insert into Comment",bufferedWriter);
			//insert Comment
			numberOfInsertions = 0;
			Comment[] comments = FileOperations.readCommentFile(
			cengTubeDBDirectory + System.getProperty("file.separator") +
					 "data" + System.getProperty("file.separator") +
					"Test_CommentFile.txt");
			
			numberOfInsertions = cengTubeDB.insertComment(comments);
			printLine(numberOfInsertions + " comments are inserted.",bufferedWriter);
			addDivider(bufferedWriter);
			/***********************************************************/
			/*******************Insert INTO Comment ends***************/
			/***********************************************************/
	
			
			/***********************************************************/
			/*******************Insert INTO Watch starts**********/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("Insert into Watch",bufferedWriter);

			numberOfInsertions = 0;
			Watch[] watchEntries = FileOperations.readWatchFile(
			cengTubeDBDirectory + System.getProperty("file.separator") +
					 "data" + System.getProperty("file.separator") +
					"Test_WatchFile.txt");
			
			numberOfInsertions = cengTubeDB.insertWatch(watchEntries);
			printLine( numberOfInsertions + " watch entries are inserted.",bufferedWriter);
			addDivider(bufferedWriter);
			/***********************************************************/
			/*******************Insert INTO Watch ends************/
			/***********************************************************/
			
			
			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("List videos which have higher likeCount than dislikeCount",bufferedWriter);
			try {
				
				QueryResult.VideoTitleLikeCountDislikeCountResult[] videoTitleLikeCountDislikeCountResultArray = cengTubeDB.question3();
				
				//Header Line
				printLine("videoTitle" + "\t" + "likeCount" + "\t" + "dislikeCount",bufferedWriter);
				
				if(videoTitleLikeCountDislikeCountResultArray != null) {
					for(QueryResult.VideoTitleLikeCountDislikeCountResult videoTitleLikeCountDislikeCountResult :
							videoTitleLikeCountDislikeCountResultArray){
						printLine(videoTitleLikeCountDislikeCountResult.toString(),bufferedWriter);
					}	
				}
				
				
			} catch(Exception e) {
				printLine("Exception occurred: \n\n" + e.toString(),bufferedWriter);
			}
			addDivider(bufferedWriter);
			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			
			
			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("List the videos commented by the given userID",bufferedWriter);
			try {
					
				QueryResult.VideoTitleUserNameCommentTextResult[] videoTitleUserNameCommentTextResultArray =
						cengTubeDB.question4(6);
				
				//Header Line
				printLine("videoTitle" + "\t" + "userName" + "\t" + "commentText", bufferedWriter);

				
				if(videoTitleUserNameCommentTextResultArray != null) {
					for(QueryResult.VideoTitleUserNameCommentTextResult videoTitleUserNameCommentTextResult
							: videoTitleUserNameCommentTextResultArray){
						printLine(videoTitleUserNameCommentTextResult.toString(), bufferedWriter);
					}
				}
				
			} catch(Exception e) {
				printLine("Exception occurred: \n\n" + e.toString(),bufferedWriter);
			}
			addDivider(bufferedWriter);
			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			
			
			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("Find the oldest published video for a given userID which doesn’t contain ”VLOG” in its title", bufferedWriter);
			try {
				
				
				QueryResult.VideoTitleUserNameDatePublishedResult[] videoTitleUserNameDatePublishedResultArray =
						cengTubeDB.question5(7);
				
				//Header Line
				printLine("videoTitle" + "\t" + "userName" + "\t" + "datePublished",bufferedWriter);

				if(videoTitleUserNameDatePublishedResultArray != null) {
						for(QueryResult.VideoTitleUserNameDatePublishedResult videoTitleUserNameDatePublishedResult
								: videoTitleUserNameDatePublishedResultArray){
						printLine(videoTitleUserNameDatePublishedResult.toString(), bufferedWriter);
					}	
				}
				
			} catch(Exception e) {
				printLine("Exception occurred: \n\n" + e.toString(),bufferedWriter);
			}
			
			addDivider(bufferedWriter);
			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			
			
			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			addDivider(bufferedWriter);

			addInputTitle("List the trending top three videos for a given time interval",bufferedWriter);
			try {


				QueryResult.VideoTitleUserNameNumOfWatchResult[] videoTitleUserNameNumOfWatchResultArray =
						cengTubeDB.question6("2011-01-01", "2021-01-01");

				printLine("videoTitle" + "\t" + "userName" + "\t" + "numOfWatch", bufferedWriter);

				if(videoTitleUserNameNumOfWatchResultArray != null) {
					for(QueryResult.VideoTitleUserNameNumOfWatchResult videoTitleUserNameNumOfWatchResult
							: videoTitleUserNameNumOfWatchResultArray){
						printLine(videoTitleUserNameNumOfWatchResult.toString(), bufferedWriter);
					}
				}

			} catch(Exception e) {
				printLine("Exception occurred: \n\n" + e.toString(),bufferedWriter);
			}

			addDivider(bufferedWriter);

			/***********************************************************/
			/***********************************************************/
			/***********************************************************/


			/***********************************************************/
			/***********************************************************/
			/***********************************************************/

			addDivider(bufferedWriter);
			addInputTitle("List users and number of videos watched only by her/him", bufferedWriter);
			try {
				
				
				QueryResult.UserIDUserNameNumOfVideosWatchedResult[] userIDUserNameNumOfVideosWatchedResultArray =
						cengTubeDB.question7();
					
				//Header Line
				printLine("userID" + "\t" + "userName" + "\t" + "numOfVideosWatched", bufferedWriter);
			
				for(QueryResult.UserIDUserNameNumOfVideosWatchedResult userIDUserNameNumOfVideosWatchedResult
						: userIDUserNameNumOfVideosWatchedResultArray){
					printLine(userIDUserNameNumOfVideosWatchedResult.toString(),bufferedWriter);
				}
					
				
			} catch(Exception e) {
				printLine("Exception occurred: \n\n" + e.toString(),bufferedWriter);
			}
			addDivider(bufferedWriter);
			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			
			
			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("List users who have watched and commented all of their own videos",bufferedWriter);
			try {
				
				QueryResult.UserIDUserNameEmailResult[] userIDUserNameEmailResultArray =
						cengTubeDB.question8();
				
				//Header Line
				printLine("userID" + "\t" + "userName" + "\t" + "email", bufferedWriter);
				
				for(QueryResult.UserIDUserNameEmailResult userIDUserNameEmailResult : userIDUserNameEmailResultArray){
					printLine(userIDUserNameEmailResult.toString(),bufferedWriter);
				}
				
			} catch(Exception e) {
				printLine("Exception occurred: \n\n" + e.toString(),bufferedWriter);
			}
			addDivider(bufferedWriter);
			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			
			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("List users that has at least one watch but no comments",bufferedWriter);
		
			try {

				QueryResult.UserIDUserNameEmailResult[] userIDUserNameEmailResultArray =
						cengTubeDB.question9();
				
				//Header Line
				printLine("userID" + "\t" + "userName" + "\t" + "email", bufferedWriter);

				for(QueryResult.UserIDUserNameEmailResult userIDUserNameEmailResult : userIDUserNameEmailResultArray){
					printLine(userIDUserNameEmailResult.toString(),bufferedWriter);
				}
				
			} catch(Exception e) {
				printLine("Exception occurred: \n\n" + e.toString(),bufferedWriter);
			}
			addDivider(bufferedWriter);

			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			
			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("Update the users’ status to ”verified” if the view count of all their videos " +
					"in total is more than the given number", bufferedWriter);

			try {
				int numOfRowsAffected = cengTubeDB.question10(10);
				printLine("Number of updated rows " + numOfRowsAffected, bufferedWriter);
			} catch (Exception e) {
				printLine("Exception occurred: \n\n" + e.toString(),bufferedWriter);

			}

			addDivider(bufferedWriter);

			/***********************************************************/
			/***********************************************************/
			/***********************************************************/

			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("Given a video id and a string, update the title of the video with the given id " +
					"to the new string", bufferedWriter);

			try {
				int numOfRowsAffected = cengTubeDB.question11(7, "When Pulp Fiction Isn't Home (Oven Kid)");
				printLine("Number of updated rows " + numOfRowsAffected, bufferedWriter);
			} catch (Exception e) {
				printLine("Exception occurred: \n\n" + e.toString(),bufferedWriter);

			}

			addDivider(bufferedWriter);

			/***********************************************************/
			/***********************************************************/
			/***********************************************************/

			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("Delete the video(s) with the given video name ",bufferedWriter);
			try {
				int numOfRowsAfterDelete = cengTubeDB.question12("Slime Challenge");
				printLine("Number of rows after delete " + numOfRowsAfterDelete, bufferedWriter);
				
			} catch(Exception e) {
				printLine("Exception occured: \n\n" + e.toString(),bufferedWriter);
			}
			addDivider(bufferedWriter);
			/***********************************************************/
			/***********************************************************/
			/***********************************************************/
			
			/***********************************************************/
			/*************Drop tables starts****************************/
			/***********************************************************/
			addDivider(bufferedWriter);
			addInputTitle("Drop tables", bufferedWriter);
			numberOfTablesDropped = 0;
			
			// Drop tables
			try {
				numberOfTablesDropped = cengTubeDB.dropTables();
			} catch(Exception e) {
				e.printStackTrace();
			}
			
			// Check if tables are dropped
			printLine("Dropped " + numberOfTablesDropped + " tables.", bufferedWriter);
			
			addDivider(bufferedWriter);
			/***********************************************************/
			/*************Drop tables ends******************************/
			/***********************************************************/
			
			
			//Close Writer
			bufferedWriter.close();
			

		} catch (IOException e1) {
			e1.printStackTrace();
		} 
	}

}
