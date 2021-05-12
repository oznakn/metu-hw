package ceng.ceng351.cengtubedb;

public class Comment {
    Integer commentID;
    Integer userID;
    Integer videoID;
    String commentText;
    String dateCommented;

    public Comment(Integer commentID, Integer userID, Integer videoID, String commentText, String dateCommented) {
        this.commentID = commentID;
        this.userID = userID;
        this.videoID = videoID;
        this.commentText = commentText;
        this.dateCommented = dateCommented;
    }

    public Integer getUserID() {
        return userID;
    }

    public Integer getVideoID() {
        return videoID;
    }

    public Integer getCommentID() {
        return commentID;
    }

    public String getCommentText() {
        return commentText;
    }

    public String getDateCommented() {
        return dateCommented;
    }
}
