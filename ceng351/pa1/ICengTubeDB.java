package ceng.ceng351.cengtubedb;

public interface ICengTubeDB {
    /**
     * Place your initialization code inside if required.
     *
     * This function will be called before all other operations. If your implementation
     * needs initialization, necessary operations should be done inside this function. For
     * example, you can set your connection to the database server inside this function.
     */

    public void initialize();


    /**
     * Should create the necessary tables.
     *
     * @return the number of tables that are created successfully.
     */
    public int createTables();


    /**
     * Should drop the tables if exists.
     *
     * @return the number of tables that are dropped successfully.
     */
    public int dropTables();


    /**
     * Should insert an array of User into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertUser(User[] users);

    /**
     * Should insert an array of Video into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertVideo(Video[] videos);

    /**
     * Should insert an array of Comment into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertComment(Comment[] comments);

    /**
     * Should insert an array of Watch into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertWatch(Watch[] watchEntries);


    /**
     * Get videos which have higher likeCount than dislikeCount
     *
     * @return QueryResult.VideoTitleLikeCountDislikeCountResult[]
     */
    public QueryResult.VideoTitleLikeCountDislikeCountResult[] question3();

    /**
     * Get the videos commented by the given userID
     *
     * @param userID given userID
     *
     * @return QueryResult.VideoTitleUserNameCommentTextResult[]
     */
    public QueryResult.VideoTitleUserNameCommentTextResult[] question4(Integer userID);

    /**
     * Find the oldest published video for a given userID which doesn’t contain ”VLOG" in its title
     *
     * @param userID given userID
     * @return QueryResult.VideoTitleUserNameDatePublishedResult[]
     */
    public QueryResult.VideoTitleUserNameDatePublishedResult[] question5(Integer userID);


    /**
     * Get the trending top three videos for a given time interval
     *
     * YOU SHOULD INCLUDE dateStart and dateEnd in the result, it is a CLOSED interval.
     *
     * @param dateStart start date of the interval
     * @param dateEnd end date of the interval
     *
     * @return QueryResult.VideoTitleUserNameNumOfWatchResult[]
     */
    public QueryResult.VideoTitleUserNameNumOfWatchResult[] question6(String dateStart, String dateEnd);


    /**
     * Get users and the number of videos watched only by her/him.
     *
     *
     * @return QueryResult.UserIDUserNameEmailResult[]
     */
    public QueryResult.UserIDUserNameNumOfVideosWatchedResult[] question7();


    /**
     * Get users who have watched and commented all of their own videos
     *
     * @return QueryResult.UserIDUserNameEmailResult[]
     */
    public QueryResult.UserIDUserNameEmailResult[]  question8();

    /**
     * Get users that has at least one watch but no comments
     *
     * @return QueryResult.UserIDUserNameEmailResult[]
     */
    public QueryResult.UserIDUserNameEmailResult[]  question9();

    /**
     *  Update the users’ status to ”verified” if the view count of all their videos in total is more than the given number
     *
     * @param givenViewCount the threshold to use in the query
     *
     * @return int return the number of rows affected
     */
    public int question10(int givenViewCount);

    /**
     *  Given a video id and a string, update the title of the video with the given id to the new string
     *
     * @param videoID the identifier for the video to be updated
     * @param newTitle the new title that will replace the old one
     *
     * @return int return the number of rows affected
     */
    public int question11(Integer videoID, String newTitle);

    /**
     *   Delete the video(s) with the given video title.
     *
     * @param videoTitle
     *
     * @return int return the number of rows in the Video table after delete operation
     */
    public int question12(String videoTitle);

}

