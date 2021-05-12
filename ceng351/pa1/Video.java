package ceng.ceng351.cengtubedb;

public class Video {
    Integer videoID;
    Integer userID;
    String videoTitle;
    Integer likeCount;
    Integer dislikeCount;
    String datePublished;

    public Video(Integer videoID, Integer userID, String videoTitle, Integer likeCount, Integer dislikeCount, String datePublished) {
        this.videoID = videoID;
        this.userID = userID;
        this.videoTitle = videoTitle;
        this.likeCount = likeCount;
        this.dislikeCount = dislikeCount;
        this.datePublished = datePublished;
    }

    public Integer getUserID() {
        return userID;
    }

    public Integer getDislikeCount() {
        return dislikeCount;
    }

    public Integer getLikeCount() {
        return likeCount;
    }

    public Integer getVideoID() {
        return videoID;
    }

    public String getDatePublished() {
        return datePublished;
    }

    public String getVideoTitle() {
        return videoTitle;
    }
}
