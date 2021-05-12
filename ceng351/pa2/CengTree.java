import java.util.ArrayList;

public class CengTree
{
    public CengTreeNode root;
    // Any extra attributes...

    public CengTree(Integer order)
    {
        CengTreeNode.order = order;
        this.root = new CengTreeNodeLeaf(null);
    }

    public void addVideo(CengVideo video)
    {
        this.root.addVideo(video, this);
    }

    public ArrayList<CengTreeNode> searchVideo(Integer key)
    {
        CengVideo video = null;

        ArrayList<CengTreeNode> result = new ArrayList<>();
        CengTreeNode head = this.root;

        while (head != null) {
            result.add(head);

            if (head.type == CengNodeType.Internal) {
                CengTreeNodeInternal nodeInternal = (CengTreeNodeInternal) head;

                head = nodeInternal.search(key);
            } else if (head.type == CengNodeType.Leaf) {
                CengTreeNodeLeaf nodeLeaf = (CengTreeNodeLeaf) head;

                video = nodeLeaf.search(key);
                break;
            }
        }

        if (video == null) {
            System.out.print("Could not find ");
            System.out.print(key);
            System.out.println(".");

            return null;
        }

        for (int i = 0; i < result.size(); i++) {
            if (result.get(i).type == CengNodeType.Internal) {
                CengTreeNodeInternal nodeInternal = (CengTreeNodeInternal) result.get(i);

                nodeInternal.printWithoutChildren(i);
            } else if (result.get(i).type == CengNodeType.Leaf) {
                CengTreeNodeLeaf nodeLeaf = (CengTreeNodeLeaf) result.get(i);

                nodeLeaf.printVideo(video, i);
            }
        }

        return result;
    }

    public void printTree()
    {
        if (this.root != null) {
            this.root.print(0);
        }
    }

    // Any extra functions...
}
