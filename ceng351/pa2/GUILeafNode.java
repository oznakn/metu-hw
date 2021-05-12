import java.awt.Color;

import javax.swing.JLabel;

@SuppressWarnings("serial")
public class GUILeafNode extends GUITreeNode
{
    public GUILeafNode(CengTreeNode node)
    {
        super(node);

        this.setBackground(Color.green);

        CengTreeNodeLeaf castNode = (CengTreeNodeLeaf)node;

        for(int i = 0; i < castNode.videoCount(); i++)
        {
            if(i >= this.labels.size())
            {
                System.out.println("Video count is greater than label count.");

                return;
            }

            Integer key = castNode.videoKeyAtIndex(i);

            String keyString = "" + key + "*";

            JLabel correspondingLabel = this.labels.get(i);

            correspondingLabel.setText(keyString);
            correspondingLabel.repaint();
        }
    }
}
