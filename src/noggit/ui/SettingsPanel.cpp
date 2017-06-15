// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#include <noggit/ui/SettingsPanel.h>

#include <noggit/TextureManager.h>
#include <util/qt/overload.hpp>


#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>


#include <algorithm>

namespace util
{
  file_line_edit::file_line_edit (mode m, QString browse_title, QWidget* parent)
    : QWidget (parent)
  {
    new QHBoxLayout (this);
    layout()->setContentsMargins (0, 0, 0, 0);

    layout()->addWidget (actual = new QLineEdit);
    auto button (new QPushButton ("Browse…", this));
    layout()->addWidget (button);

    connect ( button, &QPushButton::clicked
            , [=]
              {
                auto result
                  ( m == files
                  ? QFileDialog::getOpenFileName
                      (nullptr, browse_title, actual->text())
                  : QFileDialog::getExistingDirectory
                      (nullptr, browse_title, actual->text())
                  );
                if (!result.isNull())
                {
                  if (m == directories && !(result.endsWith ("/") || result.endsWith ("\\")))
                  {
                    result += "/";
                  }
                  actual->setText (result);
                }
              }
            );
  }
}

namespace noggit
{
  namespace ui
  {
    settings::settings()
      : QDialog (nullptr)
      , _settings (new QSettings (this))
    {
      setWindowTitle ("Settings");

      auto layout (new QFormLayout (this));

      auto browse_row
        ( [&] (util::file_line_edit** line, char const* title, QString const& key, util::file_line_edit::mode mode)
          {
            layout->addRow
              ( title
              , *line = new util::file_line_edit (mode, title, this)
              );
            connect ( (*line)->actual, &QLineEdit::textChanged
                    , [&, key] (QString value)
                      {
                        _settings->setValue (key, value);
                      }
                    );
          }
        );


      browse_row (&gamePathField, "Game Path", "project/game_path", util::file_line_edit::directories);
      browse_row (&projectPathField, "Project Path", "project/path", util::file_line_edit::directories);
      browse_row (&wodPathField, "WoD Save Path", "project/wod_save_path", util::file_line_edit::directories);
      browse_row (&importPathField, "Import Path", "project/import_file", util::file_line_edit::files);
      browse_row (&wmvLogPathField, "WMV Log Path", "project/wod_save_path", util::file_line_edit::files);      

      _mysql_box = new QGroupBox ("MySQL (uid storage)", this);
      _mysql_box->setToolTip ("Store the maps' max model unique id (uid) in a mysql database to sync your uids with different computers/users to avoid duplications");
      auto mysql_layout (new QFormLayout (_mysql_box));

#ifdef USE_MYSQL_UID_STORAGE
      mysql_box->setCheckable (true);
#else
      mysql_layout->addRow (new QLabel ("Warning /!\\"));
      mysql_layout->addRow (new QLabel ("Your noggit wasn't build with mysql, you can't use this feature"));
#endif

      _mysql_server_field = new QLineEdit (_settings->value ("project/mysql/server").toString(), this);
      _mysql_user_field = new QLineEdit (_settings->value ("project/mysql/user").toString(), this);
      _mysql_pwd_field = new QLineEdit (_settings->value ("project/mysql/pwd").toString(), this);
      _mysql_db_field = new QLineEdit (_settings->value ("project/mysql/db").toString(), this);

      mysql_layout->addRow ("Server", _mysql_server_field);
      mysql_layout->addRow ("User", _mysql_user_field);
      mysql_layout->addRow ("Password", _mysql_pwd_field);
      mysql_layout->addRow ("Database", _mysql_db_field);
      layout->addRow (_mysql_box);

      auto wireframe_box (new QGroupBox ("Wireframe", this));
      auto wireframe_layout (new QFormLayout (wireframe_box));

      _wireframe_type_group = new QButtonGroup (wireframe_box);

      auto radio_wire_full (new QRadioButton ("Full wireframe"));
      auto radio_wire_cursor (new QRadioButton ("Around cursor"));

      _wireframe_type_group->addButton (radio_wire_full, 0);
      _wireframe_type_group->addButton (radio_wire_cursor, 1);     

      wireframe_layout->addRow (new QLabel ("Type:"));
      wireframe_layout->addRow (radio_wire_full);
      wireframe_layout->addRow (radio_wire_cursor);

      _wireframe_radius = new QDoubleSpinBox (wireframe_box);
      _wireframe_radius->setRange (1.0, 100.0);

      wireframe_layout->addRow ("Radius", _wireframe_radius);
      wireframe_layout->addRow (new QLabel ("(real radius = cursor radius * wireframe radius)"));

      _wireframe_width = new QDoubleSpinBox (wireframe_box);
      _wireframe_width->setRange (0.0, 10.0);
      _wireframe_width->setSingleStep(0.1);
      wireframe_layout->addRow ("Width", _wireframe_width);

      wireframe_layout->addRow ("Color", _wireframe_color = new color_widgets::ColorSelector (wireframe_box));

      wireframe_layout->addRow ("Rainbow !", _wireframe_rainbow_cb = new QCheckBox(wireframe_box));

      layout->addRow (wireframe_box);


      layout->addRow ( "View Distance"
                     , viewDistanceField = new QDoubleSpinBox
                     );
      viewDistanceField->setRange (0.f, 1048576.f);      

      layout->addRow ( "FarZ"
                     , farZField = new QDoubleSpinBox
                     );
      farZField->setRange (0.f, 1048576.f);


      layout->addRow ("Tablet support", tabletModeCheck = new QCheckBox ("enabled", this));


      auto warning (new QWidget (this));
      new QHBoxLayout (warning);
      auto icon (new QLabel (warning));
      icon->setPixmap
        (render_blp_to_pixmap ("interface/gossipframe/availablequesticon.blp"));
      warning->layout()->addWidget (icon);
      warning->layout()->addWidget
        (new QLabel ("Changes may not take effect until next launch.", warning));
      layout->addRow (warning);

      auto buttonBox ( new QDialogButtonBox ( QDialogButtonBox::Save
                                            | QDialogButtonBox::Cancel
                                            )
                     );

      layout->addRow (buttonBox);

      connect ( buttonBox, &QDialogButtonBox::accepted
              , [this]
                {
                  hide();
                  save_changes();
                }
              );

      connect ( buttonBox, &QDialogButtonBox::rejected
              , [this]
                {
                  hide();
                  discard_changes();
                }
              );
    }

    void settings::discard_changes()
    {
      gamePathField->actual->setText (_settings->value ("project/game_path").toString());
      projectPathField->actual->setText (_settings->value ("project/path").toString());
      wodPathField->actual->setText (_settings->value ("project/wod_save_path").toString());
      importPathField->actual->setText (_settings->value ("project/import_file").toString());
      wmvLogPathField->actual->setText (_settings->value ("project/wmv_log_file").toString());
      viewDistanceField->setValue (_settings->value ("view_distance", 1000.f).toFloat());
      farZField->setValue (_settings->value ("farZ", 2048.f).toFloat());
      tabletModeCheck->setChecked (_settings->value ("tablet/enabled", false).toBool());

      _mysql_box->setChecked (_settings->value ("project/mysql/enabled").toBool());
      _mysql_server_field->setText (_settings->value ("project/mysql/server").toString());
      _mysql_user_field->setText(_settings->value ("project/mysql/user").toString());
      _mysql_pwd_field->setText (_settings->value ("project/mysql/pwd").toString());
      _mysql_db_field->setText (_settings->value ("project/mysql/db").toString());

      _wireframe_type_group->button (_settings->value ("wireframe/type", 0).toInt())->toggle();
      _wireframe_radius->setValue (_settings->value ("wireframe/radius", 1.5f).toFloat());
      _wireframe_width->setValue (_settings->value ("wireframe/width", 1.f).toFloat());
      _wireframe_color->setColor(_settings->value("wireframe/color").value<QColor>());
      _wireframe_rainbow_cb->setChecked (_settings->value ("wireframe/rainbow", 0).toInt());
    }

    void settings::save_changes()
    {
      _settings->setValue ("project/game_path", gamePathField->actual->text());
      _settings->setValue ("project/path", projectPathField->actual->text());
      _settings->setValue ("project/wod_save_path", wodPathField->actual->text());
      _settings->setValue ("project/import_file", importPathField->actual->text());
      _settings->setValue ("project/wmv_log_file", wmvLogPathField->actual->text());
      _settings->setValue ("farZ", farZField->value());
      _settings->setValue ("view_distance", viewDistanceField->value());
      _settings->setValue ("tablet/enabled", tabletModeCheck->isChecked());

      _settings->setValue ("project/mysql/enabled", _mysql_box->isChecked());
      _settings->setValue ("project/mysql/server", _mysql_server_field->text());
      _settings->setValue ("project/mysql/user", _mysql_user_field->text());
      _settings->setValue ("project/mysql/pwd", _mysql_pwd_field->text());
      _settings->setValue ("project/mysql/db", _mysql_db_field->text());

      _settings->setValue ("wireframe/type", _wireframe_type_group->checkedId());
      _settings->setValue ("wireframe/radius", _wireframe_radius->value());
      _settings->setValue ("wireframe/width", _wireframe_width->value());
      _settings->setValue ("wireframe/color", _wireframe_color->color());
      _settings->setValue ("wireframe/rainbow", (int)_wireframe_rainbow_cb->isChecked());
    }
  }
}
