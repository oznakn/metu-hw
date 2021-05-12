import java.util.ArrayList;

public class CengTreeNodeLeaf extends CengTreeNode
{
    private ArrayList<CengVideo> videos;
    // TODO: Any extra attributes

    public CengTreeNodeLeaf(CengTreeNode parent)
    {
        super(parent);

        this.type = CengNodeType.Leaf;
        this.videos = new ArrayList<>();
    }

    // GUI Methods - Do not modify
    public int videoCount()
    {
        return videos.size();
    }
    public Integer videoKeyAtIndex(Integer index)
    {
        if(index >= this.videoCount()) {
            return -1;
        } else {
            CengVideo video = this.videos.get(index);

            return video.getKey();
        }
    }

    // Extra Functions
    public CengVideo search(Integer key) {
        for (int i = 0; i < this.videos.size(); i++) {
            if (this.videos.get(i).getKey().equals(key)) {
                return this.videos.get(i);
            }
        }

        return null;
    }

    @Override
    public void addVideo(CengVideo video, CengTree tree) {
        boolean inserted = false;
        for (int i = 0; i < this.videos.size(); i++) {
            CengVideo cursor = this.videos.get(i);

            if (cursor.getKey() > video.getKey()) {
                this.videos.add(i, video);
                inserted = true;
                break;
            }
        }

        if (!inserted) {
            this.videos.add(video);
        }

        if (this.videos.size() > CengTreeNodeLeaf.order * 2) {
            int midIndex = (int) Math.floor((double) this.videos.size() / 2);

            CengVideo midVideo = this.videos.get(midIndex);

            CengTreeNodeLeaf newLeafNode = new CengTreeNodeLeaf(this.getParent());

            for (int i = midIndex; i < this.videos.size(); i++) {
                newLeafNode.videos.add(this.videos.get(i));
            }

            if (this.videos.size() > midIndex) {
                this.videos.subList(midIndex, this.videos.size()).clear();
            }

            if (this.getParent() == null) {
                CengTreeNodeInternal newRoot = new CengTreeNodeInternal(null);

                tree.root = newRoot;

                newRoot.addNode(this.videos.get(0).getKey(), this, tree);
                newRoot.addKey(midVideo.getKey());
                newRoot.addNode(midVideo.getKey(), newLeafNode, tree);
            } else {
                CengTreeNodeInternal parentNode = (CengTreeNodeInternal) this.getParent();

                parentNode.addKey(midVideo.getKey());
                parentNode.addNode(midVideo.getKey(), newLeafNode, tree);
            }
        }
    }

    @Override
    public void print(int indent) {
        this.printIndent(indent);
        System.out.println("<data>");

        for (int i = 0; i < this.videos.size(); i++) {
            this.printVideo(this.videos.get(i), indent);
        }

        this.printIndent(indent);
        System.out.println("</data>");
    }
}
