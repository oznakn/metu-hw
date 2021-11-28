import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;
import static org.antlr.v4.runtime.CharStreams.fromFileName;

public class V {
  public static void main(String[] args) {
    try {
      CharStream input = fromFileName("main.v");

      VLexer lexer = new VLexer(input);
      TokenStream tokens = new CommonTokenStream(lexer);

      VParser parser = new VParser(tokens);
      VListener listener = new VListener();

      ParseTree tree = parser.program();

      ParseTreeWalker walker = new ParseTreeWalker();
      walker.walk(listener, tree);

    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  public static class VListener extends VBaseListener {
    private int scope = 0;

    public void printScope() {
      this.printScope("");
    }

    public void printScope(String text) {
      for (int i = 0; i < scope; i++) {
        text += "\t";
      }

      System.out.print(text);
    }

    @Override
    public void enterProgram(VParser.ProgramContext ctx) {
      System.out.println("\t\tSymbolTable\n");
    }

    @Override
    public void enterDeclaration(VParser.DeclarationContext ctx) {
      this.printScope(String.format("(at line %s)\t", ctx.start.getLine()));

      System.out.print("GlobalVariable ");
    }

    @Override
    public void exitDeclaration(VParser.DeclarationContext ctx) {
      System.out.println("");
    }

    @Override
    public void enterFunction(VParser.FunctionContext ctx) {
      this.printScope(String.format("\n(at line %s)\t", ctx.start.getLine()));

      String functionName = ctx.ID().getText();
      String returnType = ctx.basicType().getText();

      String text = String.format("Function (name: %s, returnType: %s, parameters: (", functionName, returnType);

      System.out.print(text);

      scope++;
    }

	  @Override
    public void exitFunction(VParser.FunctionContext ctx) {
      scope--;
      this.printScope();
      System.out.print("\t\t}\n\n");
    }

    @Override
    public void enterParameterList(VParser.ParameterListContext ctx) {
      if (ctx.variableList() == null) {
        System.out.print("none");
      }
    }

	  @Override
    public void exitParameterList(VParser.ParameterListContext ctx) {
      System.out.println(")) {");
    }

    @Override
    public void enterVariableList(VParser.VariableListContext ctx) {
      var variableNames = ctx.ID();
      var variableTypes = ctx.type();

      for (int i = 0; i < variableNames.size(); i++) {
        String variableName = variableNames.get(i).getText();
        String variableType = variableTypes.get(i).basicType().getText();

        String text = String.format("(name: %s, type: %s)", variableName, variableType);

        if (i < variableNames.size() - 1) {
          text += ", ";
        }

        System.out.print(text);
      }
    }

    @Override
    public void enterDeclarationInFunction(VParser.DeclarationInFunctionContext ctx) {
      this.printScope(String.format("(at line %s)\t", ctx.start.getLine()));

      System.out.print("LocalVariable ");
    }

    @Override
    public void exitDeclarationInFunction(VParser.DeclarationInFunctionContext ctx) {
      System.out.println("");
    }
  }
}
