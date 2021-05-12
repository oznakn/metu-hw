package ceng.ceng351.cengtubedb;


public class User {
	
	Integer userID;
	String userName;
	String email;
	String password;
	String status;

	public User(Integer userID, String userName, String email, String password, String status) {
		this.userID = userID;
		this.userName = userName;
		this.email = email;
		this.password = password;
		this.status = status;
	}

	public Integer getUserID() {
		return userID;
	}

	public String getEmail() {
		return email;
	}

	public String getPassword() {
		return password;
	}

	public String getStatus() {
		return status;
	}

	public String getUserName() {
		return userName;
	}
}
