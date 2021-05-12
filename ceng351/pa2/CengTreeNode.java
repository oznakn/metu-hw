import java.awt.Color;

public abstract class CengTreeNode
{
    static protected Integer order;
    private CengTreeNode parent;
    protected CengNodeType type; // Type needs to be set for proper GUI. Check CengNodeType.java.

    // GUI Accessors - do not modify
    public Integer level;
    public Color color;

    // Any extra attributes, if necessary

    public CengTreeNode(CengTreeNode parent)
    {
        this.parent = parent;

        this.color = CengGUI.getRandomBorderColor();

        // TODO: Extra initializations, if necessary.
    }

    // Getters-Setters - Do not modify
    public CengTreeNode getParent()
    {
        return parent;
    }

    public void setParent(CengTreeNode parent)
    {
        this.parent = parent;
    }

    public CengNodeType getType()
    {
        return type;
    }

    // GUI Methods - Do not modify
    public Color getColor()
    {
        return this.color;
    }

    // Extra Functions

    public final String createIndent(int indent) {
        String s = "";
        for (int i = 0; i < indent; i++) {
            s += "\t";
        }
        return s;
    }

    public final void printIndent(int indent) {
        System.out.print(this.createIndent(indent));
    }

    public abstract void addVideo(CengVideo video, CengTree tree);

    public abstract void print(int indent);

    public final void printVideo(CengVideo video, int indent) {
        this.printIndent(indent);

        System.out.println("<record>" + video.getKey() + "|" + video.getVideoTitle() + "|" + video.getChannelName() + "|" + video.getCategory() + "</record>");
    }
}
