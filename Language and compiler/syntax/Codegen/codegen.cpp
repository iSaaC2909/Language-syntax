#include <iostream>
#include <string>
#include <memory>

// Define AST node types
enum class NodeType {
    Number,
    BinaryOperation,
    Variable
    // Add more node types as needed
};

// Abstract base class for AST nodes
class AstNode {
public:
    virtual ~AstNode() = default;
    virtual NodeType getType() const = 0;
};

// AST node for numbers
class NumberNode : public AstNode {
public:
    NumberNode(int value) : value(value) {}
    NodeType getType() const override { return NodeType::Number; }
    int getValue() const { return value; }

private:
    int value;
};

// AST node for binary operations
class BinaryOperationNode : public AstNode {
public:
    BinaryOperationNode(char op, std::unique_ptr<AstNode> left, std::unique_ptr<AstNode> right)
        : op(op), left(std::move(left)), right(std::move(right)) {}

    NodeType getType() const override { return NodeType::BinaryOperation; }
    char getOperator() const { return op; }
    const AstNode& getLeft() const { return *left; }
    const AstNode& getRight() const { return *right; }

private:
    char op;
    std::unique_ptr<AstNode> left;
    std::unique_ptr<AstNode> right;
};

// AST node for variables (identifiers)
class VariableNode : public AstNode {
public:
    VariableNode(const std::wstring& name) : name(name) {}
    NodeType getType() const override { return NodeType::Variable; }
    const std::wstring& getName() const { return name; }

private:
    std::wstring name;
};

// Simple code generator for the example language
class CodeGenerator {
public:
    std::wstring generateCode(const AstNode& rootNode) {
        code.clear();
        generateCodeInternal(rootNode);
        return code;
    }

private:
    void generateCodeInternal(const AstNode& node) {
        switch (node.getType()) {
            case NodeType::Number:
                generateNumberCode(static_cast<const NumberNode&>(node));
                break;
            case NodeType::BinaryOperation:
                generateBinaryOperationCode(static_cast<const BinaryOperationNode&>(node));
                break;
            case NodeType::Variable:
                generateVariableCode(static_cast<const VariableNode&>(node));
                break;
            // Add more cases for other node types if needed
        }
    }

    void generateNumberCode(const NumberNode& node) {
        code += std::to_wstring(node.getValue());
    }

    void generateBinaryOperationCode(const BinaryOperationNode& node) {
        code += L"(";
        generateCodeInternal(node.getLeft());
        code += L" " + node.getOperator() + L" ";
        generateCodeInternal(node.getRight());
        code += L")";
    }

    void generateVariableCode(const VariableNode& node) {
        code += node.getName();
    }

    std::wstring code;
};

int main() {
    // Example AST for expression: 3 + 4 * (5 - 2)
    std::unique_ptr<AstNode> rootNode = std::make_unique<BinaryOperationNode>(
        L'+',
        std::make_unique<NumberNode>(3),
        std::make_unique<BinaryOperationNode>(
            L'*',
            std::make_unique<NumberNode>(4),
            std::make_unique<BinaryOperationNode>(
                L'-',
                std::make_unique<NumberNode>(5),
                std::make_unique<NumberNode>(2)
            )
        )
    );

    CodeGenerator codeGenerator;
    std::wstring generatedCode = codeGenerator.generateCode(*rootNode);

    std::wcout << L"Generated Code: " << generatedCode << std::endl;

    return 0;
}
