import java.util.ArrayList;

public class CengTreeNodeInternal extends CengTreeNode
{
    private ArrayList<Integer> keys;
    private ArrayList<CengTreeNode> children;

    public CengTreeNodeInternal(CengTreeNode parent)
    {
        super(parent);

        this.type = CengNodeType.Internal;
        this.keys = new ArrayList<>();
        this.children = new ArrayList<>();
    }

    // GUI Methods - Do not modify
    public ArrayList<CengTreeNode> getAllChildren()
    {
        return this.children;
    }
    public Integer keyCount()
    {
        return this.keys.size();
    }
    public Integer keyAtIndex(Integer index)
    {
        if(index >= this.keyCount() || index < 0)
        {
            return -1;
        }
        else
        {
            return this.keys.get(index);
        }
    }

    // Extra Functions
    public void addKey(Integer key) {
        boolean inserted = false;
        for (int i = 0; i < this.keys.size(); i++) {
            if (this.keys.get(i) > key) {
                this.keys.add(i, key);
                inserted = true;
                break;
            }
        }

        if (!inserted) {
            this.keys.add(key);
        }
    }

    public void addNode(Integer key, CengTreeNode node, CengTree tree) {
        boolean inserted = false;
        for (int i = 0; i < this.keys.size(); i++) {
            if (this.keys.get(i) > key) {
                this.children.add(i, node);
                inserted = true;
                break;
            }
        }

        if (!inserted) {
            this.children.add(node);
        }

        node.setParent(this);

        if (this.keys.size() > CengTreeNode.order * 2) {
            int midKeyIndex = (int) Math.floor((double) this.keys.size() / 2);

            int newKey = this.keys.get(midKeyIndex);

            CengTreeNodeInternal newInternalNode = new CengTreeNodeInternal(this.getParent());

            for (int i = midKeyIndex + 1; i < this.keys.size(); i++) {
                newInternalNode.keys.add(this.keys.get(i));
            }

            if (this.keys.size() > midKeyIndex) {
                this.keys.subList(midKeyIndex, this.keys.size()).clear();
            }

            for (int i = midKeyIndex + 1; i < this.children.size(); i++) {
                this.children.get(i).setParent(newInternalNode);
                newInternalNode.children.add(this.children.get(i));
            }

            if (this.children.size() > midKeyIndex + 1) {
                this.children.subList(midKeyIndex + 1, this.children.size()).clear();
            }

            if (this.getParent() == null) {
                CengTreeNodeInternal newRoot = new CengTreeNodeInternal(null);

                tree.root = newRoot;

                newRoot.addNode(this.keys.get(0), this, tree);
                newRoot.addKey(newKey);
                newRoot.addNode(newKey, newInternalNode, tree);
            } else {
                CengTreeNodeInternal parentNode = (CengTreeNodeInternal) this.getParent();

                parentNode.addKey(newKey);
                parentNode.addNode(newKey, newInternalNode, tree);
            }
        }
    }

    public CengTreeNode search(Integer key) {
        for (int i = 0; i < this.keys.size(); i++) {
            if (this.keys.get(i) > key) {
                return this.children.get(i);
            }
        }

        return this.children.get(this.children.size() - 1);
    }

    @Override
    public void addVideo(CengVideo video, CengTree tree) {
        boolean inserted = false;

        for (int i = 0; i < this.keys.size(); i++) {
            if (this.keys.get(i) > video.getKey()) {
                this.children.get(i).addVideo(video, tree);
                inserted = true;
                break;
            }
        }

        if (!inserted) {
            this.children.get(this.children.size() - 1).addVideo(video, tree);
        }
    }

    public void printWithoutChildren(int indent) {
        this.printIndent(indent);
        System.out.println("<index>");

        for (int i = 0; i < this.keys.size(); i++) {
            this.printIndent(indent);
            System.out.println(this.keys.get(i));
        }

        this.printIndent(indent);
        System.out.println("</index>");
    }

    @Override
    public void print(int indent) {
        this.printWithoutChildren(indent);

        indent++;

        for (int i = 0; i < this.children.size(); i++) {
            this.children.get(i).print(indent);
        }
    }
}
