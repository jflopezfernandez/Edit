
#include "syntax-highlighter.h"


// ----------------------------------- Protected ------------------------------

void Highlighter::highlightBlock(const QString& text)
{
    foreach (const HighlightingRule& rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);

        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;

    if (previousBlockState() != 1) {
        startIndex = text.indexOf(commentStartExpression);
    }

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);

        int endIndex = match.capturedStart();
        int commentLength = 0;

        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }

        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

// ----------------------------------- Public ---------------------------------

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    QStringList keywordPatterns;

    // Types
    keywordPatterns << "\\bchar\\b"
                    << "\\bint\\b"
                    << "\\blong\\b"
                    << "\\bdouble\\b"
                    << "\\bfloat\\b"
                    << "\\bvoid\\b"
                    << "\\bshort\\b";

    // User-defined types
    keywordPatterns << "\\bclass\\b"
                    << "\\bstruct\\b"
                    << "\\benum\\b"
                    << "\\bunion\\b"
                    << "\\bvirtual\\b"
                    << "\\bdelete\\b"
                    << "\\bdefault\\b"
                    << "\\bpublic\\b"
                    << "\\bprotected\\b"
                    << "\\bprivate\\b"
                    << "\\bfriend\\b";

    // Storage-specifiers
    keywordPatterns << "\\bconst\\b"
                    << "\\bvolatile\\b"
                    << "\\bunsigned\\b"
                    << "\\bsigned\\b"
                    << "\\bstatic\\b";

    // Keywords
    keywordPatterns << "\\btypename\\b"
                    << "\\busing\\b"
                    << "\\bnamespace\\b"
                    << "\\btemplate\\b"
                    << "\\btypedef\\b"
                    << "\\boperator\\b"
                    << "\\binline\\b";

    // Headers
    //keywordPatterns << "\\b#include\\b";

    foreach (const QString& pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;

        highlightingRules.append(rule);
    }

    // Keyword: Class
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    // Keyword: (Single line comment)
    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    // Keyword: (Multi-line comment)
    multiLineCommentFormat.setForeground(Qt::red);

    // Keyword: (Quotation Marks)
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    // Keyword: (Function)
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    // Keyword (Comments)
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}
