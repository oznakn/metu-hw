package ceng.ceng351.cengtubedb;

import java.sql.*;
import java.util.ArrayList;

public class CengTubeDB implements ICengTubeDB {
    private static final String CONN_URL = "jdbc:mysql://144.122.71.168:3306/db230959?autoReconnect=true&useSSL=false";
    private static final String USERNAME = "e230959";
    private static final String PASSWORD = "d939a6bf";

    private static final String CREATE_TABLE_USER_SQL =
            "CREATE TABLE User (" +
                    "userID INT," +
                    "userName VARCHAR(30)," +
                    "email VARCHAR(30)," +
                    "password VARCHAR(30)," +
                    "status VARCHAR(30)," +
                    "PRIMARY KEY (userID)" +
            ");";

    private static final String CREATE_TABLE_VIDEO_SQL =
            "CREATE TABLE Video (" +
                    "videoID INT," +
                    "userID INT," +
                    "videoTitle VARCHAR(60)," +
                    "likeCount INT," +
                    "dislikeCount INT," +
                    "datePublished DATE," +
                    "PRIMARY KEY (videoID)," +
                    "FOREIGN KEY (userID) REFERENCES User(userID) ON DELETE CASCADE" +
            ");";

    private static final String CREATE_TABLE_COMMENT_SQL =
            "CREATE TABLE Comment (" +
                    "commentID INT," +
                    "userID INT," +
                    "videoID INT," +
                    "commentText VARCHAR(1000)," +
                    "dateCommented DATE," +
                    "PRIMARY KEY (commentID)," +
                    "FOREIGN KEY (userID) REFERENCES User(userID) ON DELETE SET NULL," +
                    "FOREIGN KEY (videoID) REFERENCES Video(videoID) ON DELETE CASCADE" +
            ");";

    private static final String CREATE_TABLE_WATCH_SQL =
            "CREATE TABLE Watch (" +
                    "userID INT," +
                    "videoID INT," +
                    "dateWatched DATE," +
                    "PRIMARY KEY (userID, videoID)," +
                    "FOREIGN KEY (userID) REFERENCES User(userID) ON DELETE CASCADE," +
                    "FOREIGN KEY (videoID) REFERENCES Video(videoID) ON DELETE CASCADE" +
            ");";

    private static final String[] CREATE_TABLE_SQL_LIST = new String[]{
            CREATE_TABLE_USER_SQL,
            CREATE_TABLE_VIDEO_SQL,
            CREATE_TABLE_COMMENT_SQL,
            CREATE_TABLE_WATCH_SQL,
    };

    private static final String[] DROP_TABLE_LIST = new String[]{
            "Watch",
            "Comment",
            "Video",
            "User"
    };

    private Connection conn;

    @Override
    public void initialize() {
        try {
            this.conn = DriverManager.getConnection(CONN_URL, USERNAME, PASSWORD);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    @Override
    public int createTables() {
        int createdTables = 0;

        for (String sql : CREATE_TABLE_SQL_LIST) {
            try {
                conn.prepareStatement(sql).execute();

                createdTables += 1;
            }   catch (SQLException e) {
                e.printStackTrace();
            }
        }

        return createdTables;
    }

    @Override
    public int dropTables() {
        int dropedTables = 0;

        for (String tableName : DROP_TABLE_LIST) {
            try {
                String sql = "DROP TABLE " + tableName + ";";

                conn.prepareStatement(sql).execute();

                dropedTables += 1;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }

        try {
            this.conn.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return dropedTables;
    }

    @Override
    public int insertUser(User[] users) {
        int insertedRows = 0;

        for (User user: users) {
            try {
                String sql = "INSERT INTO User (userID, userName, email, password, status) VALUES (?, ?, ?, ?, ?);";
                PreparedStatement statement = this.conn.prepareStatement(sql);

                statement.setInt(1, user.userID);
                statement.setString(2, user.userName);
                statement.setString(3, user.email);
                statement.setString(4, user.password);
                statement.setString(5, user.status);

                statement.execute();

                insertedRows += 1;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }

        return insertedRows;
    }

    @Override
    public int insertVideo(Video[] videos) {
        int insertedRows = 0;

        for (Video video: videos) {
            try {
                String sql = "INSERT INTO Video (videoID, userID, videoTitle, likeCount, dislikeCount, datePublished) VALUES (?, ?, ?, ?, ?, ?);";
                PreparedStatement statement = this.conn.prepareStatement(sql);

                statement.setInt(1, video.videoID);
                statement.setInt(2, video.userID);
                statement.setString(3, video.videoTitle);
                statement.setInt(4, video.likeCount);
                statement.setInt(5, video.dislikeCount);
                statement.setString(6, video.datePublished);

                statement.execute();

                insertedRows += 1;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }

        return insertedRows;
    }

    @Override
    public int insertComment(Comment[] comments) {
        int insertedRows = 0;

        for (Comment comment: comments) {
            try {
                String sql = "INSERT INTO Comment (commentID, userID, videoID, commentText, dateCommented) VALUES (?, ?, ?, ?, ?);";
                PreparedStatement statement = this.conn.prepareStatement(sql);

                statement.setInt(1, comment.commentID);
                statement.setInt(2, comment.userID);
                statement.setInt(3, comment.videoID);
                statement.setString(4, comment.commentText);
                statement.setString(5, comment.dateCommented);

                statement.execute();

                insertedRows += 1;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }

        return insertedRows;
    }

    @Override
    public int insertWatch(Watch[] watchEntries) {
        int insertedRows = 0;

        for (Watch watch: watchEntries) {
            try {
                String sql = "INSERT INTO Watch (userID, videoID, dateWatched) VALUES (?, ?, ?);";
                PreparedStatement statement = this.conn.prepareStatement(sql);

                statement.setInt(1, watch.userID);
                statement.setInt(2, watch.videoID);
                statement.setString(3, watch.dateWatched);

                statement.execute();

                insertedRows += 1;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }

        return insertedRows;
    }

    @Override
    public QueryResult.VideoTitleLikeCountDislikeCountResult[] question3() {
        String sql = "SELECT DISTINCT V.videoTitle, V.likeCount, V.dislikeCount FROM Video V " +
                "WHERE V.likeCount > V.dislikeCount " +
                "ORDER BY V.videoTitle";

        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            ResultSet resultSet =  statement.executeQuery();
            ArrayList<QueryResult.VideoTitleLikeCountDislikeCountResult> results = new ArrayList<>();

            while (resultSet.next()) {
                results.add(new QueryResult.VideoTitleLikeCountDislikeCountResult(
                        resultSet.getString("videoTitle"),
                        resultSet.getInt("likeCount"),
                        resultSet.getInt("dislikeCount")
                ));
            }

            return results.toArray(new QueryResult.VideoTitleLikeCountDislikeCountResult[0]);
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return new QueryResult.VideoTitleLikeCountDislikeCountResult[0];
    }

    @Override
    public QueryResult.VideoTitleUserNameCommentTextResult[] question4(Integer userID) {
        String sql = "SELECT DISTINCT V.videoTitle, U.userName, C.commentText " +
                "FROM Comment C, Video V, User U " +
                "WHERE C.videoID = V.videoID AND C.userID = U.userID AND C.userID = ? " +
                "ORDER BY V.videoTitle";

        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            statement.setInt(1, userID);

            ResultSet resultSet =  statement.executeQuery();
            ArrayList<QueryResult.VideoTitleUserNameCommentTextResult> results = new ArrayList<>();

            while (resultSet.next()) {
                results.add(new QueryResult.VideoTitleUserNameCommentTextResult(
                        resultSet.getString("videoTitle"),
                        resultSet.getString("userName"),
                        resultSet.getString("commentText")
                ));
            }

            return results.toArray(new QueryResult.VideoTitleUserNameCommentTextResult[0]);
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return new QueryResult.VideoTitleUserNameCommentTextResult[0];
    }

    @Override
    public QueryResult.VideoTitleUserNameDatePublishedResult[] question5(Integer userID) {
        String sql = "SELECT DISTINCT V.videoTitle, U.userName, V.datePublished " +
                "FROM Video V, User U " +
                "WHERE V.userID = U.userID AND V.videoTitle NOT LIKE '%VLOG%' AND U.userID = ? " +
                "AND V.datePublished = (SELECT MIN(V2.datePublished) " +
                    "FROM Video V2, User U2 " +
                    "WHERE V2.userID = U2.userID AND V2.videoTitle NOT LIKE '%VLOG%' AND U2.userID = ?) " +
                "ORDER BY V.datePublished, V.videoTitle";

        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            statement.setInt(1, userID);
            statement.setInt(2, userID);

            ResultSet resultSet =  statement.executeQuery();

            if (resultSet.next()) {
                return new QueryResult.VideoTitleUserNameDatePublishedResult[]{
                        new QueryResult.VideoTitleUserNameDatePublishedResult(
                                resultSet.getString("videoTitle"),
                                resultSet.getString("userName"),
                                resultSet.getString("datePublished")
                        )
                };
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return new QueryResult.VideoTitleUserNameDatePublishedResult[0];
    }

    @Override
    public QueryResult.VideoTitleUserNameNumOfWatchResult[] question6(String dateStart, String dateEnd) {
        String sql = "SELECT V.videoTitle, U.userName, " +
                "(SELECT COUNT(*) FROM Watch W WHERE W.videoID = V.videoID AND W.dateWatched >= ? AND W.dateWatched <= ?) as watchCount " +
                "FROM Video V, User U " +
                "WHERE U.userID = V.userID " +
                "ORDER BY watchCount DESC " +
                "LIMIT 3";

        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            statement.setString(1, dateStart);
            statement.setString(2, dateEnd);

            ResultSet resultSet =  statement.executeQuery();
            ArrayList<QueryResult.VideoTitleUserNameNumOfWatchResult> results = new ArrayList<>();

            while (resultSet.next()) {
                results.add(new QueryResult.VideoTitleUserNameNumOfWatchResult(
                        resultSet.getString("videoTitle"),
                        resultSet.getString("userName"),
                        resultSet.getInt("watchCount")
                ));
            }

            return results.toArray(new QueryResult.VideoTitleUserNameNumOfWatchResult[0]);
        } catch (SQLException e) {
            e.printStackTrace();
        }
        
        return new QueryResult.VideoTitleUserNameNumOfWatchResult[0];
    }

    @Override
    public QueryResult.UserIDUserNameNumOfVideosWatchedResult[] question7() {
        String sql = "SELECT U.userID, U.userName, " +
                "(SELECT COUNT(*) FROM Video V " +
                        "WHERE NOT EXISTS (SELECT W.videoID, W.userID FROM Watch W WHERE W.videoID = V.videoID AND W.userID != U.userID) AND " +
                        "EXISTS (SELECT W.videoID, W.userID FROM Watch W WHERE W.videoID = V.videoID AND W.userID = U.userID) " +
                ") as watchCount " +
                "FROM User U " +
                "HAVING watchCount != 0 " +
                "ORDER BY U.userID";

        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            ResultSet resultSet =  statement.executeQuery();
            ArrayList<QueryResult.UserIDUserNameNumOfVideosWatchedResult> results = new ArrayList<>();

            while (resultSet.next()) {
                results.add(new QueryResult.UserIDUserNameNumOfVideosWatchedResult(
                        resultSet.getInt("userID"),
                        resultSet.getString("userName"),
                        resultSet.getInt("watchCount")
                ));
            }

            return results.toArray(new QueryResult.UserIDUserNameNumOfVideosWatchedResult[0]);
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return new QueryResult.UserIDUserNameNumOfVideosWatchedResult[0];
    }

    @Override
    public QueryResult.UserIDUserNameEmailResult[] question8() {
        String sql = "SELECT U.userID, U.userName, U.email " +
                "FROM User U " +
                "WHERE NOT EXISTS " +
                    "(SELECT V.videoID FROM Video V " +
                        "WHERE V.userID = U.userID AND " +
                        "(NOT EXISTS (SELECT W.videoID, W.userID FROM Watch W WHERE W.videoID = V.videoID AND W.userID = U.userID) " +
                        "OR " +
                        "NOT EXISTS (SELECT C.videoID, C.userID FROM Comment C WHERE C.videoID = V.videoID AND C.userID = U.userID))" +
                    ") " +
                "AND (SELECT COUNT(V.videoID) FROM Video V WHERE V.userID = U.userID) > 0 " +
                "ORDER BY U.userID";

        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            ResultSet resultSet =  statement.executeQuery();
            ArrayList<QueryResult.UserIDUserNameEmailResult> results = new ArrayList<>();

            while (resultSet.next()) {
                results.add(new QueryResult.UserIDUserNameEmailResult(
                        resultSet.getInt("userID"),
                        resultSet.getString("userName"),
                        resultSet.getString("email")
                ));
            }

            return results.toArray(new QueryResult.UserIDUserNameEmailResult[0]);
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return new QueryResult.UserIDUserNameEmailResult[0];
    }

    @Override
    public QueryResult.UserIDUserNameEmailResult[] question9() {
        String sql = "SELECT U.userID, U.userName, U.email " +
                "FROM User U " +
                "WHERE EXISTS (SELECT W.videoID FROM Watch W WHERE W.userID = U.userID) AND " +
                "NOT EXISTS (SELECT C.videoID FROM Comment C WHERE C.userID = U.userID) " +
                "ORDER BY U.userID";

        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            ResultSet resultSet =  statement.executeQuery();
            ArrayList<QueryResult.UserIDUserNameEmailResult> results = new ArrayList<>();

            while (resultSet.next()) {
                results.add(new QueryResult.UserIDUserNameEmailResult(
                        resultSet.getInt("userID"),
                        resultSet.getString("userName"),
                        resultSet.getString("email")
                ));
            }

            return results.toArray(new QueryResult.UserIDUserNameEmailResult[0]);
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return new QueryResult.UserIDUserNameEmailResult[0];
    }

    @Override
    public int question10(int givenViewCount) {
        String sql = "UPDATE User U SET U.status = 'verified' " +
                "WHERE" +
                "(SELECT COUNT(*) FROM Watch W WHERE W.videoID IN (SELECT V.videoID FROM Video V WHERE V.userID = U.userID)) > ?";

        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            statement.setInt(1, givenViewCount);

            return statement.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return 0;
    }

    @Override
    public int question11(Integer videoID, String newTitle) {
        String sql = "UPDATE Video V SET V.videoTitle = ? WHERE V.videoID = ?";

        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            statement.setString(1, newTitle);
            statement.setInt(2, videoID);

            return statement.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return 0;
    }

    @Override
    public int question12(String videoTitle) {
        String sql = "DELETE FROM Video WHERE videoTitle = ?";

        try {
            PreparedStatement statement = this.conn.prepareStatement(sql);

            statement.setString(1, videoTitle);

            statement.executeUpdate();

            try {
                statement = this.conn.prepareStatement("SELECT COUNT(*) FROM Video");

                ResultSet resultSet = statement.executeQuery();

                if (resultSet.next()) {
                    return resultSet.getInt(1);
                }
            } catch (SQLException e) {
                e.printStackTrace();
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return 0;
    }
}
