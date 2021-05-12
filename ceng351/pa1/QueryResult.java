/**
 * 
 */
package ceng.ceng351.cengtubedb;

/**
 * This class will be used for returning query results.
 * You will design queries which return different columns.
 * You can find an appropriate sub-class for each result type.
 */

public class QueryResult {

	public static class VideoTitleLikeCountDislikeCountResult {
		private String videoTitle;
		private Integer likeCount;
		private Integer dislikeCount;

		public VideoTitleLikeCountDislikeCountResult(String videoTitle, Integer likeCount, Integer dislikeCount) {
			this.videoTitle = videoTitle;
			this.likeCount = likeCount;
			this.dislikeCount = dislikeCount;
		}

		public String getVideoTitle() {
			return videoTitle;
		}

		public void setVideoTitle(String videoTitle) {
			this.videoTitle = videoTitle;
		}

		public Integer getLikeCount() {
			return likeCount;
		}

		public void setLikeCount(Integer likeCount) {
			this.likeCount = likeCount;
		}

		public Integer getDislikeCount() {
			return dislikeCount;
		}

		public void setDislikeCount(Integer dislikeCount) {
			this.dislikeCount = dislikeCount;
		}

		public String toString() {
			return videoTitle + "\t" + likeCount + "\t" + dislikeCount;
		}

	}

	public static class VideoTitleUserNameCommentTextResult {
		private String videoTitle;
		private String userName;
		private String commentText;

		public VideoTitleUserNameCommentTextResult(String videoTitle, String userName, String commentText) {
			this.videoTitle = videoTitle;
			this.userName = userName;
			this.commentText = commentText;
		}

		public String getVideoTitle() {
			return videoTitle;
		}

		public void setVideoTitle(String videoTitle) {
			this.videoTitle = videoTitle;
		}

		public String getUserName() {
			return userName;
		}

		public void setUserName(String userName) {
			this.userName = userName;
		}

		public String getCommentText() {
			return commentText;
		}

		public void setCommentText(String commentText) {
			this.commentText = commentText;
		}

		public String toString() {
			return videoTitle + "\t" + userName + "\t" + commentText;
		}
	}

	public static class VideoTitleUserNameDatePublishedResult {
		String videoTitle;
		String userName;
		String datePublished;

		public VideoTitleUserNameDatePublishedResult(String videoTitle, String userName, String datePublished) {
			this.videoTitle = videoTitle;
			this.userName = userName;
			this.datePublished = datePublished;
		}

		public String getVideoTitle() {
			return videoTitle;
		}

		public void setVideoTitle(String videoTitle) {
			this.videoTitle = videoTitle;
		}

		public String getUserName() {
			return userName;
		}

		public void setUserName(String userName) {
			this.userName = userName;
		}

		public String getDatePublished() {
			return datePublished;
		}

		public void setDatePublished(String datePublished) {
			this.datePublished = datePublished;
		}

		public String toString() {
			return videoTitle + "\t" + userName + "\t" + datePublished;
		}
	}

	public static class VideoTitleUserNameNumOfWatchResult {
		String videoTitle;
		String userName;
		Integer numOfWatch;

		public VideoTitleUserNameNumOfWatchResult(String videoTitle, String userName, Integer numOfWatch) {
			this.videoTitle = videoTitle;
			this.userName = userName;
			this.numOfWatch = numOfWatch;
		}

		public String getVideoTitle() {
			return videoTitle;
		}

		public void setVideoTitle(String videoTitle) {
			this.videoTitle = videoTitle;
		}

		public String getUserName() {
			return userName;
		}

		public void setUserName(String userName) {
			this.userName = userName;
		}

		public Integer getNumOfWatch() {
			return numOfWatch;
		}

		public void setNumOfWatch(Integer numOfWatch) {
			this.numOfWatch = numOfWatch;
		}

		public String toString() {
			return videoTitle + "\t" + userName + "\t" + numOfWatch;
		}
	}

	public static class UserIDUserNameNumOfVideosWatchedResult {
		private Integer userID;
		private String userName;
		private Integer numOfVideosWatched;

		public UserIDUserNameNumOfVideosWatchedResult(Integer userID, String userName, Integer numOfVideosWatched) {
			this.userID = userID;
			this.userName = userName;
			this.numOfVideosWatched = numOfVideosWatched;
		}

		public Integer getUserID() {
			return userID;
		}

		public void setUserID(Integer userID) {
			this.userID = userID;
		}

		public String getUserName() {
			return userName;
		}

		public void setUserName(String userName) {
			this.userName = userName;
		}

		public Integer getNumOfVideosWatched() {
			return numOfVideosWatched;
		}

		public void setNumOfVideosWatched(Integer numOfVideosWatched) {
			this.numOfVideosWatched = numOfVideosWatched;
		}

		public String toString() {
			return userID + "\t" + userName + "\t" + numOfVideosWatched;
		}
	}

	public static class UserIDUserNameEmailResult {
	
		private Integer userID;
		private String userName;
		private String email;

		public UserIDUserNameEmailResult(Integer userID, String userName, String email) {
			this.userID = userID;
			this.userName = userName;
			this.email = email;
		}

		public Integer getUserID() {
			return userID;
		}

		public void setUserID(Integer userID) {
			this.userID = userID;
		}

		public String getUserName() {
			return userName;
		}

		public void setUserName(String userName) {
			this.userName = userName;
		}

		public String getEmail() {
			return email;
		}

		public void setEmail(String email) {
			this.email = email;
		}

		public String toString() {
			return userID + "\t" + userName + "\t" + email;
		}

	}
	

	
}
