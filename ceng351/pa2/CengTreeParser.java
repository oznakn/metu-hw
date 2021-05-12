import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public class CengTreeParser
{
    public static ArrayList<CengVideo> parseVideosFromFile(String filename)
    {
        ArrayList<CengVideo> videoList = new ArrayList<CengVideo>();

        try {
            File text = new File(filename);
            Scanner scan = new Scanner(text);

            while(scan.hasNextLine()){
                String line = scan.nextLine();
                String[] params = line.split("\\|");

                videoList.add(new CengVideo(
                        Integer.parseInt(params[0]),
                        params[1],
                        params[2],
                        params[3]
                ));
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        }

        return videoList;
    }

    public static void startParsingCommandLine() throws IOException
    {
        Scanner scan = new Scanner(System.in);

        while (scan.hasNextLine()) {
            String line = scan.nextLine();

            String[] params = line.split("\\|");

            String command = params[0];

            if (command.equals("quit")) {
                break;
            } else if (command.equals("add")) {
                CengVideo cengVideo = new CengVideo(
                        Integer.parseInt(params[1]),
                        params[2],
                        params[3],
                        params[4]
                );

                CengVideoRunner.addVideo(cengVideo);
            } else if (command.equals("search")) {
                CengVideoRunner.searchVideo(Integer.parseInt(params[1]));
            } else if (command.equals("print")) {
                CengVideoRunner.printTree();
            }
        }
    }
}
