#include "backgroundsettingsitem.h"

#include <QPainter>
#include "graphicresources/fontmanager.h"
#include "dpiscalemanager.h"

namespace PreferencesWindow {

BackgroundSettingsItem::BackgroundSettingsItem(ScalableGraphicsObject *parent) : BaseItem(parent, 50),
    isExpanded_(false), curDescrTextOpacity_(0.0)
{
    setFlags(flags() | QGraphicsItem::ItemClipsChildrenToShape | QGraphicsItem::ItemIsFocusable);

    comboBoxMode_ = new ComboBoxItem(this, QT_TRANSLATE_NOOP("PreferencesWindow::BackgroundSettingsItem", "App Background"), "", 50, /*QColor(255, 0, 0)*/Qt::transparent, 0, false);

    comboBoxMode_->addItem(tr("Country flags"), BACKGROUND_TYPE_COUNTRY_FLAGS);
    comboBoxMode_->addItem(tr("None"), BACKGROUND_TYPE_NONE);
    comboBoxMode_->addItem(tr("Custom"), BACKGROUND_TYPE_CUSTOM);
    comboBoxMode_->setCurrentItem(BACKGROUND_TYPE_COUNTRY_FLAGS);
    connect(comboBoxMode_, SIGNAL(currentItemChanged(QVariant)), SLOT(onBackgroundModeChanged(QVariant)));

    imageItemDisconnected_ = new SelectImageItem(this, tr("Disconnected"), true);
    connect(imageItemDisconnected_, SIGNAL(pathChanged(QString)), SLOT(onDisconnectedPathChanged(QString)));
    imageItemConnected_ = new SelectImageItem(this, tr("Connected"), false);
    connect(imageItemConnected_, SIGNAL(pathChanged(QString)), SLOT(onConnectedPathChanged(QString)));

    dividerLine_ = new DividerLine(this, 276);

    expandEnimation_.setStartValue(COLLAPSED_HEIGHT);
    expandEnimation_.setEndValue(EXPANDED_HEIGHT);
    expandEnimation_.setDuration(150);
    connect(&expandEnimation_, SIGNAL(valueChanged(QVariant)), SLOT(onExpandAnimationValueChanged(QVariant)));
}

void BackgroundSettingsItem::onExpandAnimationValueChanged(const QVariant &value)
{
    setHeight(value.toInt()* G_SCALE);

    qreal progress = (qreal)(value.toInt() - COLLAPSED_HEIGHT) / (qreal)(EXPANDED_HEIGHT - COLLAPSED_HEIGHT);
    curDescrTextOpacity_ = progress;
    dividerLine_->setPos(24 * G_SCALE, value.toInt() * G_SCALE - dividerLine_->boundingRect().height());
}

void BackgroundSettingsItem::onLanguageChanged()
{
}

void BackgroundSettingsItem::onDisconnectedPathChanged(const QString &path)
{
    curBackgroundSettings_.backgroundImageDisconnected = path;
    emit backgroundSettingsChanged(curBackgroundSettings_);
}

void BackgroundSettingsItem::onConnectedPathChanged(const QString &path)
{
    curBackgroundSettings_.backgroundImageConnected = path;
    emit backgroundSettingsChanged(curBackgroundSettings_);
}

void BackgroundSettingsItem::hideOpenPopups()
{
    comboBoxMode_->hideMenu();
}

void BackgroundSettingsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void BackgroundSettingsItem::setBackgroundSettings(const types::BackgroundSettings &settings)
{
    if (curBackgroundSettings_ != settings)
    {
        curBackgroundSettings_ = settings;
        if (curBackgroundSettings_.backgroundType == BACKGROUND_TYPE_CUSTOM)
        {
            isExpanded_ = true;
            onExpandAnimationValueChanged(EXPANDED_HEIGHT);
        }
        else
        {
            isExpanded_ = false;
            onExpandAnimationValueChanged(COLLAPSED_HEIGHT);
        }
        comboBoxMode_->setCurrentItem((int)curBackgroundSettings_.backgroundType);
        imageItemDisconnected_->setPath(curBackgroundSettings_.backgroundImageDisconnected);
        imageItemConnected_->setPath(curBackgroundSettings_.backgroundImageConnected);
    }
}

void BackgroundSettingsItem::updateScaling()
{
    BaseItem::updateScaling();

    //update state/positions
    onExpandAnimationValueChanged(isExpanded_ ? EXPANDED_HEIGHT : COLLAPSED_HEIGHT);
    imageItemDisconnected_->setPos(0, (COLLAPSED_HEIGHT - 5)*G_SCALE);
    imageItemConnected_->setPos(0, (COLLAPSED_HEIGHT)*G_SCALE + imageItemDisconnected_->boundingRect().height());
}

void BackgroundSettingsItem::onBackgroundModeChanged(QVariant v)
{
    if (v.toInt() == BACKGROUND_TYPE_CUSTOM && !isExpanded_)
    {
        expandEnimation_.setDirection(QVariantAnimation::Forward);
        if (expandEnimation_.state() != QVariantAnimation::Running)
        {
            expandEnimation_.start();
        }
        isExpanded_ = true;
    }
    else if (isExpanded_)
    {
        expandEnimation_.setDirection(QVariantAnimation::Backward);
        if (expandEnimation_.state() != QVariantAnimation::Running)
        {
            expandEnimation_.start();
        }
        isExpanded_ = false;
    }

    if (curBackgroundSettings_.backgroundType != v.toInt())
    {
        curBackgroundSettings_.backgroundType = (BACKGROUND_TYPE)v.toInt();
        emit backgroundSettingsChanged(curBackgroundSettings_);
    }
}


} // namespace PreferencesWindow
