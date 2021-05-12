import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.DefaultTableModel;
import javax.swing.JScrollPane;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Random;

public class CengGUI
{
    private JFrame frame;

    private JTable currentVideoTable;
    private JTable alreadyAddedVideoTable;

    private static final Object[] columnNames = new Object[]{"Key", "VideoTitle", "ChannelName", "Category"};

    private ArrayList<CengVideo> currentVideos;
    private ArrayList<CengVideo> addedVideos;

    private JPanel mainModel;

    private ArrayList<GUILevel> paintedLevels;

    public static Integer orderN; // orderN = 2 * order + 1

    @SuppressWarnings("serial")
    static ArrayList<Color> definedColors = new ArrayList<Color>(){{

        // Pre-defined colors
        add(Color.black);
        //add(Color.white); Color of empty padding
        add(Color.red);
        //add(Color.green); Color of leaf background
        add(Color.blue);
        add(Color.cyan);
        add(Color.gray);
        //add(Color.lightGray); Color of internal background
        add(Color.magenta);
        add(Color.orange);
        add(Color.pink);
        add(Color.yellow);

        // Common colors
        add(new Color(128, 0, 0)); // Maroon
        add(new Color(0, 128, 0)); // Olive
        add(new Color(0, 0, 128)); // Navy
        add(new Color(0, 128, 128)); // Teal

        // Uncommon colors

        add(new Color(240,230,140)); // Khaki
        add(new Color(0,100,0)); // Dark green
        add(new Color(255,140,0)); // Dark orange
        add(new Color(47,79,79)); // Dark slate gray
        add(new Color(0,206,209)); // Dark turquoise
        add(new Color(188,143,143)); // Rosy brown
    }};

    public CengGUI()
    {
        initialize();
    }

    public void show()
    {
        frame.setVisible(true);
    }

    public void modelNeedsUpdate(CengTreeNode root)
    {
        //System.out.println("Updating model...");

        updateMainModel(root);
    }

    public void updateWithSearchResult(ArrayList<CengTreeNode> visitedNodes, Integer key)
    {
        //System.out.println("Searching for " + key);

        for(GUILevel level : paintedLevels) {
            level.paintSearchedNodes(visitedNodes);
        }
    }

    private void initialize()
    {
        currentVideos = new ArrayList<CengVideo>();
        addedVideos = new ArrayList<CengVideo>();

        frame = new JFrame();
        frame.setSize(512, 384);
        frame.setLocationRelativeTo(null);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.getContentPane().setLayout(new BoxLayout(frame.getContentPane(), BoxLayout.Y_AXIS));

        addMainModel();

        addCurrentTable();

        addAlreadyAddedTable();

        frame.pack();
    }

    private void addMainModel()
    {
        mainModel = new JPanel();
        mainModel.setLayout(new BoxLayout(mainModel, BoxLayout.Y_AXIS));

        frame.getContentPane().add(mainModel);
    }

    private void updateMainModel(CengTreeNode root)
    {
        mainModel.removeAll();

        Queue<CengTreeNode> queue = new LinkedList<CengTreeNode>();

        queue.add(root);

        ArrayList<CengTreeNode> allLevels = new ArrayList<CengTreeNode>();

        root.level = 1;

        Integer maxLevel = root.level;

        while(queue.size() > 0) {
            CengTreeNode currentNode = queue.poll();

            if(currentNode.getType() == CengNodeType.Internal) {
                CengTreeNodeInternal castNode = (CengTreeNodeInternal)currentNode;

                ArrayList<CengTreeNode> allChildren = castNode.getAllChildren();

                for(CengTreeNode node : allChildren) {
                    node.level = currentNode.level + 1;

                    maxLevel = node.level;

                    queue.add(node);
                }
            }

            allLevels.add(currentNode);
        }

        if(paintedLevels != null) {
            paintedLevels.clear();
            paintedLevels = null;
        }

        paintedLevels = new ArrayList<GUILevel>();

        for(int i = 0; i < maxLevel; i++) {
            GUILevel newLevel = new GUILevel();

            for(CengTreeNode node : allLevels) {
                if(node.level == i + 1) { // Level starts with 1
                    newLevel.addNode(node);
                }
            }

            mainModel.add(newLevel);
            paintedLevels.add(newLevel);
        }

        mainModel.revalidate();
        mainModel.repaint();

        frame.revalidate();
        frame.repaint();

        if(CengVideoRunner.shouldPack()) {
            frame.pack();
        }
    }

    private void addCurrentTable()
    {
        JPanel tablePanel = new JPanel();
        tablePanel.setLayout(new BoxLayout(tablePanel, BoxLayout.Y_AXIS));

        DefaultTableModel currentVideoTableModel = new DefaultTableModel(columnNames, 0);

        ArrayList<CengVideo> inputVideos = CengTreeParser.parseVideosFromFile(CengVideoRunner.getFilenameToParse());

        for(CengVideo video : inputVideos) {
            currentVideoTableModel.addRow(new Object[]{video.getKey(), video.getVideoTitle(), video.getChannelName(), video.getCategory()});
        }

        if(currentVideos.size() == 0) {
            currentVideos.addAll(inputVideos);
        }

        currentVideoTable = new JTable(currentVideoTableModel)
        {
            // Anon class
            private static final long serialVersionUID = 1L;

            public boolean isCellEditable(int row, int column)
            {
                return false;
            };
        };

        modifyTable(currentVideoTable);

        final JButton addButton = new JButton("Add Selected Video");

        addButton.addActionListener(new ActionListener()
                {
                    @Override
                    public void actionPerformed(ActionEvent e)
                    {
                        addSelectedVideo();
                    }
                });

        addButton.setAlignmentX(Component.CENTER_ALIGNMENT);
        tablePanel.add(addButton);
        tablePanel.add(currentVideoTable.getTableHeader());
        tablePanel.add(currentVideoTable);

        JScrollPane scrollPaneCurrentVideos = new JScrollPane(tablePanel);

        // Change width manually
        // scrollPaneCurrentVideos.getViewport().setPreferredSize(new Dimension(400, scrollPaneCurrentVideos.getViewport().getPreferredSize().height));

        frame.getContentPane().add(scrollPaneCurrentVideos);
    }

    void addAlreadyAddedTable()
    {
        JPanel tablePanel = new JPanel();
        tablePanel.setLayout(new BoxLayout(tablePanel, BoxLayout.Y_AXIS));

        DefaultTableModel addedVideoTableModel = new DefaultTableModel(columnNames, 0);

        alreadyAddedVideoTable = new JTable(addedVideoTableModel)
        {
            // Anon class
            private static final long serialVersionUID = 1L;

            public boolean isCellEditable(int row, int column)
            {
                return false;
            };
        };

        modifyTable(alreadyAddedVideoTable);

        final JButton searchButton = new JButton("Search Selected Video");

        searchButton.addActionListener(new ActionListener()
                {
                    @Override
                    public void actionPerformed(ActionEvent e)
                    {
                        searchSelectedVideo();
                    }
                });

        searchButton.setAlignmentX(Container.CENTER_ALIGNMENT);
        tablePanel.add(searchButton);
        tablePanel.add(alreadyAddedVideoTable.getTableHeader());
        tablePanel.add(alreadyAddedVideoTable);

        JScrollPane scrollPaneAddedVideo = new JScrollPane(tablePanel);

        frame.getContentPane().add(scrollPaneAddedVideo);
    }

    private void addSelectedVideo()
    {
        if(currentVideoTable.getSelectedRow() == -1) {
            return;
        }

        CengVideo selectedVideo = getSelectedVideoFromTable(currentVideoTable);

        System.out.println("add|" + selectedVideo.fullName()); // Only for visual purposes

        CengVideoRunner.addVideo(selectedVideo);

        currentVideos.remove(selectedVideo);

        addVideoToTable(selectedVideo, alreadyAddedVideoTable);

        addedVideos.add(selectedVideo);

        removeSelectionFromTable(currentVideoTable);
    }

    private void searchSelectedVideo()
    {
        if(alreadyAddedVideoTable.getSelectedRow() == -1) {
            return;
        }

        CengVideo selectedVideo = getSelectedVideoFromTable(alreadyAddedVideoTable);

        System.out.println("search|" + selectedVideo.getKey()); // Only for visual purposes

        CengVideoRunner.searchVideo(selectedVideo.getKey());
    }

    // Helpers

    private void removeSelectionFromTable(JTable table)
    {
        Integer selectedRowIndex = table.getSelectedRow();

        DefaultTableModel tableModel = (DefaultTableModel) table.getModel();
        tableModel.removeRow(selectedRowIndex);
    }

    private CengVideo getSelectedVideoFromTable(JTable table)
    {
        Integer selectedRowIndex = table.getSelectedRow();

        if(table.equals(currentVideoTable)) {
            return currentVideos.get(selectedRowIndex);
        } else {
            return addedVideos.get(selectedRowIndex);
        }
    }

    private void addVideoToTable(CengVideo video, JTable table)
    {
        DefaultTableModel tableModel = (DefaultTableModel) table.getModel();

        tableModel.addRow(new Object[]{video.getKey(), video.getVideoTitle(), video.getChannelName(), video.getCategory()});
    }

    private void modifyTable(final JTable table)
    {
        table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

        final DefaultTableModel tableModel = (DefaultTableModel) table.getModel();

        if(tableModel.getRowCount() != 0) {
            // Initialize table with first row selected.
            table.addRowSelectionInterval(0, 0);
        }

        tableModel.addTableModelListener(new TableModelListener() {
            @Override
            public void tableChanged(TableModelEvent e)
            {
                SwingUtilities.invokeLater(new Runnable()
                        {
                            @Override
                            public void run()
                            {
                                if(tableModel.getRowCount() != 0) {
                                    table.addRowSelectionInterval(0, 0);
                                }
                            }
                        });
            }
        });
    }

    public static Color getRandomBorderColor()
    {
        Random rand = new Random();

        if(CengGUI.definedColors.size() > 0) {
            int randomIndex = rand.nextInt(CengGUI.definedColors.size());

            Color pickedColor = CengGUI.definedColors.get(randomIndex);

            CengGUI.definedColors.remove(pickedColor);

            return pickedColor;
        } else {
            // Will produce only bright / light colors

            float r = rand.nextFloat() / 2f;
            float g = rand.nextFloat() / 2f;
            float b = rand.nextFloat() / 2f;

            return new Color(r, g, b).brighter();
        }
    }
}
