#include "precisionconverters.h"
#include <algorithm>
#include <optional>
#include <qlist.h>
#include <qnamespace.h>
#include <type_traits>

namespace { 

QString trimmed(const QString& input) { 
    QString result = input.trimmed();
    result.remove(' ');
    result.remove('\t');
    return result;
}

std::optional<plane_geometry::ExactScalar> parseExact(QString text, QString* error) {
    text = trimmed(text);
    if (text.isEmpty()) {
        if (error) *error = "Value cannot be empty.";
        return std::nullopt;
    }
    try {
        return plane_geometry::ExactScalar{text.toStdString()};
    } catch (const std::exception& ex) {
        if (error) *error = QString::fromUtf8(ex.what());
        return std::nullopt;
    }
}

} // namespace

namespace precision_converter {

std::optional<ExactPoint> parsePoint(const QString& text, QString* error) {
    const QString trimmedText = trimmed(text);
    if (!trimmedText.startsWith('(') || !trimmedText.endsWith(')')) {
        if (error) *error = "Point must be in the format (x,y).";
        return std::nullopt;
    }
    const QStringList parts = trimmedText.mid(1, trimmedText.size() - 2).split(',', Qt::SkipEmptyParts);
    if (parts.size() != 2) {
        if (error) *error = "Point must contains two coordinates.";
        return std::nullopt;
    }
    auto x = parseExact(parts[0], error);
    if (!x) return std::nullopt;
    auto y = parseExact(parts[1], error);
    if (!y) return std::nullopt;
    return ExactPoint{*x, *y};
}

std::optional<ExactSegment> parseSegment(const QString& startText, const QString& endText, QString* error) {
    auto start = parsePoint(startText, error);
    if (!start) return std::nullopt;
    auto end = parsePoint(endText, error);
    if (!end) return std::nullopt;
    return ExactSegment{*start, *end};
}

std::optional<plane_geometry::ExactScalar> parseScalar(const QString& text, QString* error) {
    return parseExact(text, error);
}

QString formatScalar(const plane_geometry::ExactScalar& value, int digits) {
    std::string str = value.convert_to<std::string>();
    if (digits > 0 && static_cast<int>(str.size()) > digits) { 
        str = str.substr(0, digits);
    }
    return QString::fromStdString(str);
}

} // namespace precision_converter