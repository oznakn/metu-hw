package ceng.ceng351.cengtubedb;

public class Watch {
    Integer userID;
    Integer videoID;
    String dateWatched;

    public Watch(Integer userID, Integer videoID, String dateWatched) {
        this.userID = userID;
        this.videoID = videoID;
        this.dateWatched = dateWatched;
    }

    public Integer getVideoID() {
        return videoID;
    }

    public Integer getUserID() {
        return userID;
    }

    public String getDateWatched() {
        return dateWatched;
    }
}
